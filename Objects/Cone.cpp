#include "bakkesmod/wrappers/canvaswrapper.h"
#include "Cone.h"
#include "Matrix3.h"
#include "../Extra/RenderingMath.h"
#include "../Extra/WrapperStructsExtensions.h"
#include <vector>

RT::Cone::Cone()
	: location(Vector()), direction(Vector(0,0,1)), radius(5), height(20), rollAmount(0), segments(8), thickness(1) {}

RT::Cone::Cone(Vector loc, Vector dir)
	: Cone() { location = loc; direction = dir; }

void RT::Cone::Draw(CanvasWrapper canvas) const
{
	Vector dir = direction;
	dir.normalize();
	Quat orientation = LookAt(location, location + dir, LookAtAxis::AXIS_UP).ToQuat();
	
	//Create base circle
	std::vector<Vector> basePoints;
	Vector start = {1,0,0};

	//Get all the vertices that comprise the circle
	for(int i = 0; i < segments; ++i)
	{
		Vector newPoint = start;
		float angle = ((2.f * CONST_PI_F) / segments * i) + rollAmount;
		Quat rotAmount = AngleAxisRotation(angle, Vector{0,0,1});
		newPoint = RotateVectorWithQuat(newPoint, rotAmount);
		basePoints.push_back(newPoint);
	}

	Vector2F tip = canvas.ProjectF(location + (dir * height));
	
	//Orient circle and project to canvas
	std::vector<Vector2F> canvasPoints;
	for(size_t i = 0; i != basePoints.size(); ++i)
	{
		basePoints[i] = basePoints[i] * radius;
		basePoints[i] = RotateVectorWithQuat(basePoints[i], orientation);
		basePoints[i] = basePoints[i] + location;
		canvasPoints.push_back(canvas.ProjectF(basePoints[i]));
	}

	//Draw lines
	Vector2F startPoint = {0,0}, endPoint = {0,0};
	for(size_t i = 0; i != canvasPoints.size(); ++i)
	{
		if(i < canvasPoints.size()-1)
		{
			startPoint = canvasPoints[i];
			endPoint = canvasPoints[i+1];
		}
		if(i == canvasPoints.size()-1)
		{
			startPoint = canvasPoints[i];
			endPoint = canvasPoints[0];
		}

		if(thickness == 1)
		{
			canvas.DrawLine(startPoint, endPoint);
			canvas.DrawLine(startPoint, tip);
		}
		else
		{
			canvas.DrawLine(startPoint, endPoint, thickness);
			canvas.DrawLine(startPoint, tip, thickness);
		}
	}
}
