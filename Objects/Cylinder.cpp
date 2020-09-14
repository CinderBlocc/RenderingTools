#include "bakkesmod/wrappers/canvaswrapper.h"
#include "Cylinder.h"
#include "Frustum.h"
#include "Line.h"
#include "Matrix3.h"
#include "../Extra/RenderingMath.h"
#include "../Extra/WrapperStructsExtensions.h"
#include <vector>

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
		Quat rotAmount = AngleAxisRotation(2.f * CONST_PI_F * j / segments, Vector(0,0,1));
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

	Vector cylinderBottom = this->location - (rotMat.up * height * .5f);
	Vector cylinderTop = this->location + (rotMat.up * height * .5f);

	Vector projectedLocation = VectorProjection(objLocation - cylinderBottom, cylinderTop - cylinderBottom) + cylinderBottom;

	//Check if projected point is contained within the cylinder's height
	if(Line(cylinderBottom, cylinderTop).IsPointWithinLineSegment(projectedLocation))
	{
		//Check if the distance from the original point to the projected point is less than the cylinder's radius
		if((projectedLocation - objLocation).magnitude() <= this->radius)
		{
			return true;
		}
	}
	
    return false;
}

bool RT::Cylinder::LineCrossesCylinder(const Line &line) const
{
    //Get the local to world space transformation
    Quat inverse = this->orientation.conjugate();
    Vector offset(0, 0, this->height * .5f);

    //Create a copy of the cylinder, but translated so that the base is on 0,0,0 with no rotation
    RT::Cylinder cylinderCopy = *this;
    cylinderCopy.orientation = inverse * cylinderCopy.orientation;
    cylinderCopy.location = offset;

    //Create a copy of the line, but translated to match the new orientation of the copied cylinder
    RT::Line lineCopy = line;
    lineCopy.lineBegin = lineCopy.lineBegin + offset - this->location;
    lineCopy.lineEnd = lineCopy.lineEnd + offset - this->location;
    lineCopy.lineBegin = RotateVectorWithQuat(lineCopy.lineBegin, inverse);
    lineCopy.lineEnd = RotateVectorWithQuat(lineCopy.lineEnd, inverse);

    //Test if both of the points are above, or if both are below the cylinder
    if((lineCopy.lineBegin.Z > this->height && lineCopy.lineEnd.Z > this->height) || (lineCopy.lineBegin.Z < 0 && lineCopy.lineEnd.Z < 0))
    {
        return false;
    }

    //Test if either of the points are inside the cylinder
    if(cylinderCopy.IsInCylinder(lineCopy.lineBegin) || cylinderCopy.IsInCylinder(lineCopy.lineEnd))
    {
        return true;
    }

    //Create planes to test the end caps. Draw them for testing
    RT::Plane top( Vector(0, 0, 1), -this->height );
    if(top.LineIntersectsWithPlane(lineCopy))
    {
        Vector intersectionPoint = top.LinePlaneIntersectionPoint(lineCopy);
        float dist = (intersectionPoint - Vector(0, 0, this->height)).magnitude();
        if(dist <= this->radius)
        {
            if(lineCopy.IsPointWithinLineSegment(intersectionPoint))
            {
                return true;
            }
        }
    }

    RT::Plane bottom( Vector(0, 0, 1), 0 );
    if(bottom.LineIntersectsWithPlane(lineCopy))
    {
        Vector intersectionPoint = bottom.LinePlaneIntersectionPoint(lineCopy);
        float dist = (intersectionPoint - Vector(0, 0, 0)).magnitude();
        if(dist <= this->radius)
        {
            if(lineCopy.IsPointWithinLineSegment(intersectionPoint))
            {
                return true;
            }
        }
    }

    //Test the body of the cylinder
    //Remove the vertical component from the line, then test against a 2D circle
    RT::Line flatLine = lineCopy;
    flatLine.lineBegin.Z = 0;
    flatLine.lineEnd.Z = 0;

    //Find closest point on flat line to the circle center
    Vector centerToFlatA = Vector(0, 0, 0) - flatLine.lineBegin;
    Vector flatBToFlatA = flatLine.lineEnd - flatLine.lineBegin;
    Vector closestPointOnFlatLine = RT::VectorRejection(centerToFlatA, flatBToFlatA) * -1.f;
    
    //Get that point as a percentage along the flat line
    //Use this perc on the original line to find the Z position?
    float closestPointPercAlongLine = flatLine.PointPercentageAlongLine(closestPointOnFlatLine);

    //Determine if the point on the flat line closest to the cylinder center is within the cylinder's radius
    //Ensure the point is within the line segment
    if(closestPointOnFlatLine.magnitude() < this->radius && flatLine.IsPointWithinLineSegment(closestPointOnFlatLine))
    {
        //Line crosses 2D circle
        //Check if 3D version of closestPoint is within cylinder bounds
        Vector closestPoint = lineCopy.GetPointAlongLine(closestPointPercAlongLine);
        if(closestPoint.Z >= 0 && closestPoint.Z <= height)
        {
            return true;
        }
    }

    //Default false if none of the above results in an intersection
    return false;
}
