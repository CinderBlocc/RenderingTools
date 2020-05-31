#include "Cylinder.h"
#include "Frustum.h"
#include "Line.h"
#include "../Extra/RenderingMath.h"
#include "../Extra/WrapperStructsExtensions.h"
#define _USE_MATH_DEFINES
#include <math.h>

void RT::Cylinder::Draw(CanvasWrapper canvas, Frustum &frustum, int segments)
{
	//Simple frustum check. Not very clean but can be improved later
	if(!frustum.IsInFrustum(location, height * .5f))
	{
		return;
	}

	//Create circle
	vector<Vector> circlePoints;
	for(int j = 0; j != segments; ++j)
	{
		Quat rotAmount = AngleAxisRotation(2 * M_PI * ((float)j / segments), Vector(0,0,1));
		circlePoints.push_back(RotateVectorWithQuat(Vector(radius, 0, 0), rotAmount));
	}

	//Transform circles
	vector<Vector2F> bottomPoints;
	vector<Vector2F> topPoints;
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

bool RT::Cylinder::IsInCylinder(Vector objLocation)
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
