#include "Circle.h"
#include "Matrix3.h"
#include "Frustum.h"
#include "../Extra/RenderingMath.h"
#include "../Extra/WrapperStructsExtensions.h"
#define _USE_MATH_DEFINES
#include <math.h>

void RT::Circle::Draw(CanvasWrapper canvas, Frustum &frustum)
{
	vector<Vector> circlePoints;
	Vector start = {1,0,0};
	Vector axis = {0,0,1};

	//Rename variables for easier readability
	/*float radius = circle.radius;
	Vector location = circle.location;
	Quat orientation = circle.orientation;
	float lineThickness = circle.lineThickness;
	int steps = circle.steps;
	float piePercentage = circle.piePercentage;*/

	//Get all the vertices that comprise the circle
	for(int i = 0; i < steps; ++i)
	{
		Vector newPoint = start;
		float angle = (2 * M_PI / steps) * (float)i;
		Quat rotAmount = AngleAxisRotation(angle, axis);
		newPoint = RotateVectorWithQuat(newPoint, rotAmount);
		circlePoints.push_back(newPoint);
	}

	//Reorient all points of circle
	for(size_t i = 0; i < circlePoints.size(); i++)
		circlePoints[i] = RotateVectorWithQuat(circlePoints[i], orientation);

	//Determine how many lines to draw
	int newPointAmount = ((float)circlePoints.size() * piePercentage);
	if(piePercentage != 0 && piePercentage != 1)
		newPointAmount += 1;
	
	//Calculate how much of the last line should be drawn
	float percentagePerLineSegment = 1.f / steps;
	float fullLinePercentage = (float)newPointAmount * percentagePerLineSegment - percentagePerLineSegment;
	if(piePercentage == 0)
		fullLinePercentage = 0;
	if(piePercentage == 1)
		fullLinePercentage = 1;
	float lastLineRemainder = piePercentage - fullLinePercentage;
	float lastLinePercent = lastLineRemainder/percentagePerLineSegment;
	if(piePercentage == 1)
		lastLinePercent = 1;

	//Draw the line segments
	for(size_t i = 0; i < newPointAmount; ++i)
	{
		Vector startPoint;
		Vector originalEnd;
		Vector calculatedEnd;

		if(i < circlePoints.size()-1)
		{
			startPoint = location + circlePoints[i] * radius;
			originalEnd = location + circlePoints[i + 1] * radius;
		}
		if(i == circlePoints.size()-1)
		{
			startPoint = location + circlePoints[i] * radius;
			originalEnd = location + circlePoints[0] * radius;
		}

		calculatedEnd = originalEnd;
		if(i == (size_t)newPointAmount - 1)
			calculatedEnd = ((originalEnd - startPoint) * lastLinePercent) + startPoint;

		if(frustum.IsInFrustum(startPoint) && frustum.IsInFrustum(calculatedEnd))
		{
			if(lineThickness == 1)
				canvas.DrawLine(canvas.ProjectF(startPoint), canvas.ProjectF(calculatedEnd));//avoid gaps between lines
			else
				canvas.DrawLine(canvas.ProjectF(startPoint), canvas.ProjectF(calculatedEnd), lineThickness);
		}
	}
}

void RT::Circle::DrawSegmented(CanvasWrapper canvas, Frustum &frustum, int segments, float percentPerSeg)
{
	//Horribly inefficient but whatever - duplicates all circle points for each circle
	//Fix it to use piePercentage methods multiple times around ONE circle to avoid creating duplicate calculations

	Circle circ = *this;
	circ.piePercentage = percentPerSeg / segments;
	float rotSection = (2 * M_PI) / segments;

	Matrix3 orientationMat(circ.orientation);

	for(int i = 0; i < segments; ++i)
	{
		Quat rotAroundUp = AngleAxisRotation(rotSection * i, orientationMat.up);
		circ.orientation = rotAroundUp * circ.orientation;
		circ.Draw(canvas, frustum);
	}
}
