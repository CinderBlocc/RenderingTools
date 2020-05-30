#include "Sphere.h"
#include "Line.h"
#include "Frustum.h"
#include "../Extra/RenderingMath.h"
#include "../Extra/WrapperStructsExtensions.h"
#define _USE_MATH_DEFINES
#include <math.h>

void RT::Sphere::Draw(CanvasWrapper canvas, Frustum &frustum, Vector cameraLocation, int segments)
{
	//Occlude opposite side of sphere using IsOccludingLine on each vertex with a sphere that is slightly smaller than this one
	//Create a vertical (half?) circle and rotate it a few times. Keep track of all points at each level

	//Start with sphere center at 0,0,0
	//radius of radius*.95

	//height segs == segments/2

	if(segments < 4)
	{
		segments = 4;
	}
	int maxVerticalSegs = segments / 2;

	vector<vector<Vector>> semicircles;
	Vector start = {0, 0, radius * .95f};


	//Fill vertices
	for(int i = 0; i != segments; ++i)
	{
		vector<Vector> semicirclePoints;
		for(int j = 0; j != maxVerticalSegs+1; ++j)
		{
			Vector newPoint = start;

			//Define semicircle
			Quat circleShape = AngleAxisRotation(M_PI * ((float)j / maxVerticalSegs), Vector(1,0,0));
			newPoint = RotateVectorWithQuat(newPoint, circleShape);

			//Rotate semicircle
			Quat semicirclePosition = AngleAxisRotation(2 * M_PI * ((float)i / segments), Vector(0,0,1));
			newPoint = RotateVectorWithQuat(newPoint, semicirclePosition);

			//Rotate to align to orientation
			newPoint = RotateVectorWithQuat(newPoint, orientation);

			//Translate to align to location
			newPoint = newPoint + location;

			semicirclePoints.push_back(newPoint);
		}

		semicircles.push_back(semicirclePoints);
	}

	//Draw sphere
	Sphere testSphere = *this;
	testSphere.radius *= 0.9f;
	for(size_t i = 0; i != semicircles.size(); ++i)
	{
		for(size_t j = 0; j != semicircles[i].size() - 1; ++j)
		{
			//Check if the vertical line points are visible
			if(!frustum.IsInFrustum(semicircles[i][j]) || testSphere.IsOccludingLine(Line(semicircles[i][j], cameraLocation)))
			{
				continue;
			}

			//Draw vertical line
			if(frustum.IsInFrustum(semicircles[i][j + 1]) && !testSphere.IsOccludingLine(Line(semicircles[i][j + 1], cameraLocation)))
			{
				canvas.DrawLine(canvas.ProjectF(semicircles[i][j]), canvas.ProjectF(semicircles[i][j + 1]));
			}

			//If it's the first line, there are no horizontal lines to draw
			if(j == 0)
			{
				continue;
			}

			//Draw horizontal lines
			if(i != semicircles.size() - 1)
			{
				if(frustum.IsInFrustum(semicircles[i + 1][j]) && !testSphere.IsOccludingLine(Line(semicircles[i + 1][j], cameraLocation)))
				{
					canvas.DrawLine(canvas.ProjectF(semicircles[i][j]), canvas.ProjectF(semicircles[i + 1][j]));
				}
			}
			else
			{
				//Connect last semicircle to first semicircle
				if(frustum.IsInFrustum(semicircles[0][j]) && !testSphere.IsOccludingLine(Line(semicircles[0][j], cameraLocation)))
				{
					canvas.DrawLine(canvas.ProjectF(semicircles[i][j]), canvas.ProjectF(semicircles[0][j]));
				}
			}
		}
	}
}

bool RT::Sphere::IsOccludingLine(Line line)
{
	//Checks if a line drawn from a point to the camera is occluded by the sphere

	//DrawingLocation == line.lineBegin
	//CameraLocation == line.lineEnd

	Vector cameraToSphere = location - line.lineEnd;
	Vector pointToSphere = location - line.lineBegin;
	Vector pointToCamera = line.lineEnd - line.lineBegin;

	//Check if either the drawing or the camera is inside the object
	if(cameraToSphere.magnitude() <= radius || pointToSphere.magnitude() <= radius)
		return true;

	//Check if the distance from sphere center to nearest point on the line is greater than sphere radius
	Vector midProjection = VectorProjection(pointToSphere, pointToCamera) + line.lineBegin;
	if((location - midProjection).magnitude() > radius)
		return false;

	//Check if the projected point is within the drawingToCamera line segment
	Line drawingToCameraSegment(line.lineBegin, line.lineEnd);
	
	//The projected point is on the line
	if(drawingToCameraSegment.IsPointWithinLineSegment(midProjection))
		return true;

	float perc = drawingToCameraSegment.PointPercentageAlongLine(midProjection);
	//The projected point is in front of the line. Check if camera is within sphere.
	if(perc > 1)
	{
		if(cameraToSphere.magnitude() > radius)
			return false;
		else
			return true;
	}

	//The projected point is behind the line. Check if drawing is within sphere.
	if(perc < 0)
	{
		if(pointToSphere.magnitude() > radius)
			return false;
		else
			return true;
	}
}
