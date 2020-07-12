#include "Cylinder.h"
#include "Frustum.h"
#include "Line.h"
#include "../Extra/RenderingMath.h"
#include "../Extra/WrapperStructsExtensions.h"
#define _USE_MATH_DEFINES
#include <math.h>

RT::Cylinder::Cylinder()
	: location(Vector()), orientation(Quat()), radius(50), height(100), lineThickness(1) {}

RT::Cylinder::Cylinder(float rad, float h)
	: location(Vector()), orientation(Quat()), radius(rad), height(h), lineThickness(1) {}

RT::Cylinder::Cylinder(Vector loc, float rad, float h)
	: location(loc), orientation(Quat()), radius(rad), height(h), lineThickness(1) {}

RT::Cylinder::Cylinder(Vector loc, Quat rot, float rad, float h)
	: location(loc), orientation(rot), radius(rad), height(h), lineThickness(1) {}

void RT::Cylinder::Draw(CanvasWrapper canvas, Frustum &frustum, int segments) const
{
	//Simple frustum check. Not very clean but can be improved later
	if(!frustum.IsInFrustum(location, height * .5f))
	{
		return;
	}

	//Create circle
	std::vector<Vector> circlePoints;
	for(int j = 0; j != segments; ++j)
	{
		Quat rotAmount = AngleAxisRotation(static_cast<float>(2 * M_PI * ((float)j / segments)), Vector(0,0,1));
		circlePoints.push_back(RotateVectorWithQuat(Vector(radius, 0, 0), rotAmount));
	}

	//Transform circles
	std::vector<Vector2F> bottomPoints;
	std::vector<Vector2F> topPoints;
	for(size_t i = 0; i != circlePoints.size(); ++i)
	{
		Vector bottom = circlePoints[i];
		Vector top = circlePoints[i];

		//Push circles to height of cylinder
		bottom.Z -= height * .5f;
		top.Z += height * .5f;

		//Rotate points to cylinder orientation
		bottom = RotateVectorWithQuat(bottom, orientation);
		top = RotateVectorWithQuat(top, orientation);

		//Translate to final location
		bottomPoints.push_back(canvas.ProjectF(bottom + location));
		topPoints.push_back(canvas.ProjectF(top + location));
	}

	//Draw lines
	for(size_t i = 0; i != bottomPoints.size(); ++i)
	{
		if(lineThickness == 1)
		{
			canvas.DrawLine(bottomPoints[i], topPoints[i]);
			if(i == bottomPoints.size() - 1)
			{
				//Draw from last to 0
				canvas.DrawLine(bottomPoints[i], bottomPoints[0]);
				canvas.DrawLine(topPoints[i], topPoints[0]);
			}
			else
			{
				canvas.DrawLine(bottomPoints[i], bottomPoints[i + 1]);
				canvas.DrawLine(topPoints[i], topPoints[i + 1]);
			}
		}
		else
		{
			canvas.DrawLine(bottomPoints[i], topPoints[i], lineThickness);
			if(i == bottomPoints.size() - 1)
			{
				//Draw from last to 0
				canvas.DrawLine(bottomPoints[i], bottomPoints[0], lineThickness);
				canvas.DrawLine(topPoints[i], topPoints[0], lineThickness);
			}
			else
			{
				canvas.DrawLine(bottomPoints[i], bottomPoints[i + 1], lineThickness);
				canvas.DrawLine(topPoints[i], topPoints[i + 1], lineThickness);
			}
		}
	}
}

bool RT::Cylinder::IsInCylinder(Vector objLocation) const
{
	Matrix3 rotMat(this->orientation);

	Vector cylinderBottom = this->location - rotMat.up * height * .5f;
	Vector cylinderTop = this->location + rotMat.up * height * .5f;

	Vector projectedLocation = VectorProjection(objLocation - cylinderBottom, cylinderTop - cylinderBottom) + cylinderBottom;

	//Check if projected point is contained within the cylinder's height
	if(Line(cylinderBottom, cylinderTop).IsPointWithinLineSegment(projectedLocation))
	{
		//Check if the distance from the original point to the projected point is less than the cylinder's radius
		if((projectedLocation - objLocation).magnitude() <= this->radius)
		{
			return true;
		}
		else
		{
			return false;
	
		}
	}
	else
	{
		return false;
	}
}

bool RT::Cylinder::LineCrossesCylinder(const Line &line) const
{
    /*
    https://stackoverflow.com/questions/4078401/trying-to-optimize-line-vs-cylinder-intersection
    The cylinder is circular, right? You could transform coordinates so that the center line of the cylinder functions as the Z axis.
    Then you have a 2D problem of intersecting a line with a circle.
    The intersection points will be in terms of a parameter going from 0 to 1 along the length of the line,
    so you can calculate their positions in that coordinate system and compare to the top and bottom of the cylinder.
    */

    //Create a plane at both caps of the cylinder and do a disc check
    //i.e. get intersection point, then check distance to center


    //Move objects to world origin. Remove rotation from cylinder and line points
    Quat inverse = orientation.conjugate();
    Vector offset(0, 0, height * .5f);

    RT::Cylinder cylCopy = *this;
    cylCopy.orientation = inverse * cylCopy.orientation;
    cylCopy.location = offset;

    RT::Line lineCopy = line;
    lineCopy.lineBegin = RT::RotateVectorWithQuat(lineCopy.lineBegin, inverse) - location + offset;
    lineCopy.lineEnd = RT::RotateVectorWithQuat(lineCopy.lineEnd, inverse) - location + offset;

    //Test if either of the points are inside the cylinder
    if(cylCopy.IsInCylinder(lineCopy.lineBegin) || cylCopy.IsInCylinder(lineCopy.lineEnd))
    {
        return true;
    }

    //Create planes to test the end caps
    RT::Plane top(Vector(0,0,1), height);
    RT::Plane bottom(Vector(0,0,1), 0);

    if(top.LineIntersectsWithPlane(lineCopy))
    {
        Vector intersectionPoint = top.LinePlaneIntersectionPoint(lineCopy);
        float dist = (intersectionPoint - Vector(0, 0, height)).magnitude();
        if(dist <= radius)
        {
            if(lineCopy.IsPointWithinLineSegment(intersectionPoint))
            {
                return true;
            }
        }
    }
    if(bottom.LineIntersectsWithPlane(lineCopy))
    {
        Vector intersectionPoint = bottom.LinePlaneIntersectionPoint(lineCopy);
        float dist = (intersectionPoint - Vector(0, 0, 0)).magnitude();
        if(dist <= radius)
        {
            if(lineCopy.IsPointWithinLineSegment(intersectionPoint))
            {
                return true;
            }
        }
    }

    //Test the body of the cylinder
    //Remove the vertical component from the line, then test against a 2D circle
    float beginZ = lineCopy.lineBegin.Z;
    float endZ = lineCopy.lineEnd.Z;
    lineCopy.lineBegin.Z = 0;
    lineCopy.lineEnd.Z = 0;

    //Find closest point on line to the circle center
    Vector circToA = Vector(0, 0, 0) - lineCopy.lineBegin;
    Vector BToA = lineCopy.lineEnd - lineCopy.lineBegin;
    Vector closestPoint = RT::VectorProjection(circToA, BToA);
    if(closestPoint.magnitude() < radius)
    {
        //Line crosses 2D circle, test if it is within vertical bounds of cylinder
        float perc = lineCopy.PointPercentageAlongLine(closestPoint);
        lineCopy.lineBegin.Z = beginZ;
        lineCopy.lineEnd.Z = endZ;

        Vector newClosestPoint = lineCopy.GetPointAlongLine(perc);
        if(newClosestPoint.Z <= height)
        {
            return true;
        }
    }

    return false;
}
