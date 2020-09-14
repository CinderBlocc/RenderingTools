#include "bakkesmod/wrappers/canvaswrapper.h"
#include "Cube.h"
#include "Matrix3.h"

RT::Cube::Cube()
	: location(Vector()), orientation(Quat()), sideLength(50), lineThickness(1) {}

RT::Cube::Cube(Vector loc)
    : location(loc), orientation(Quat()), sideLength(50), lineThickness(1) {}

RT::Cube::Cube(Vector loc, float sLength)
    : location(loc), orientation(Quat()), sideLength(sLength), lineThickness(1) {}

RT::Cube::Cube(Vector loc, Quat rot, float sLength)
	: location(loc), orientation(rot), sideLength(sLength), lineThickness(1) {}

void RT::Cube::Draw(CanvasWrapper canvas) const
{
	float halfLength = sideLength * .5f;

	Matrix3 matrix(orientation);
	Vector fwd = matrix.forward * halfLength;
	Vector right = matrix.right * halfLength;
	Vector up = matrix.up * halfLength;

	Vector2F points[8];
	points[0] = canvas.ProjectF(location + fwd + right + up); // Front Right Top
	points[1] = canvas.ProjectF(location + fwd + right - up); // Front Right Bottom
	points[2] = canvas.ProjectF(location + fwd - right - up); // Front Left Bottom
	points[3] = canvas.ProjectF(location + fwd - right + up); // Front Left Top
	points[4] = canvas.ProjectF(location - fwd + right + up); // Back Right Top
	points[5] = canvas.ProjectF(location - fwd + right - up); // Back Right Bottom
	points[6] = canvas.ProjectF(location - fwd - right - up); // Back Left Bottom
	points[7] = canvas.ProjectF(location - fwd - right + up); // Back Left Top

	//Rendering works differently when thickness is not 1
	if(lineThickness != 1)
	{
		for(int i = 0; i < 4; ++i)
		{
			if(i == 3)
			{
				canvas.DrawLine(points[i], points[0], lineThickness);
				canvas.DrawLine(points[i + 4], points[4], lineThickness);
				canvas.DrawLine(points[0], points[4], lineThickness);
			}
			else
			{
				canvas.DrawLine(points[i], points[i + 1], lineThickness);
				canvas.DrawLine(points[i + 4], points[i + 5], lineThickness);
				canvas.DrawLine(points[i + 1], points[i + 5], lineThickness);
			}
		}
	}
	else
	{
		for(int i = 0; i < 4; ++i)
		{
			if(i == 3)
			{
				canvas.DrawLine(points[i], points[0]);
				canvas.DrawLine(points[i + 4], points[4]);
				canvas.DrawLine(points[0], points[4]);
			}
			else
			{
				canvas.DrawLine(points[i], points[i + 1]);
				canvas.DrawLine(points[i + 4], points[i + 5]);
				canvas.DrawLine(points[i + 1], points[i + 5]);
			}
		}
	}
}
