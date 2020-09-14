#include "bakkesmod/wrappers/canvaswrapper.h"
#include "Sphere.h"
#include "Line.h"
#include "Frustum.h"
#include "../Extra/RenderingMath.h"
#include "../Extra/WrapperStructsExtensions.h"
#include <vector>

RT::Sphere::Sphere()
	: location(Vector(0,0,0)), orientation(Quat(1,0,0,0)), radius(100) {}

RT::Sphere::Sphere(Vector loc, float rad)
	: location(loc), orientation(Quat()), radius(rad) {}

RT::Sphere::Sphere(Vector loc, Quat rot, float rad)
	: location(loc), orientation(rot), radius(rad) {}

void RT::Sphere::Draw(CanvasWrapper canvas, Frustum &frustum, Vector cameraLocation, int segments) const
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

	std::vector<std::vector<Vector>> semicircles;
	Vector start = {0, 0, radius * .95f};


	//Fill vertices
	for(int i = 0; i != segments; ++i)
	{
		std::vector<Vector> semicirclePoints;
		for(int j = 0; j != maxVerticalSegs+1; ++j)
		{
			Vector newPoint = start;

			//Define semicircle
			Quat circleShape = AngleAxisRotation(CONST_PI_F * j / maxVerticalSegs, Vector(1,0,0));
			newPoint = RotateVectorWithQuat(newPoint, circleShape);

			//Rotate semicircle
			Quat semicirclePosition = AngleAxisRotation(2.f * CONST_PI_F * i / segments, Vector(0,0,1));
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
            Line vertLinePoints(semicircles[i][j], cameraLocation);
			if(!frustum.IsInFrustum(semicircles[i][j]) || testSphere.IsOccludingLine(vertLinePoints))
			{
				continue;
			}

			//Draw vertical line
            Line vertLineNext(semicircles[i][j + 1], cameraLocation);
			if(frustum.IsInFrustum(semicircles[i][j + 1]) && !testSphere.IsOccludingLine(vertLineNext))
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
                Line horizontalLine(semicircles[i + 1][j], cameraLocation);
				if(frustum.IsInFrustum(semicircles[i + 1][j]) && !testSphere.IsOccludingLine(horizontalLine))
				{
					canvas.DrawLine(canvas.ProjectF(semicircles[i][j]), canvas.ProjectF(semicircles[i + 1][j]));
				}
			}
			else
			{
				//Connect last semicircle to first semicircle
                Line connectLastToFirstLine(semicircles[0][j], cameraLocation);
				if(frustum.IsInFrustum(semicircles[0][j]) && !testSphere.IsOccludingLine(connectLastToFirstLine))
				{
					canvas.DrawLine(canvas.ProjectF(semicircles[i][j]), canvas.ProjectF(semicircles[0][j]));
				}
			}
		}
	}
}

bool RT::Sphere::IsOccludingLine(Line &line) const
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

    return false;
}
