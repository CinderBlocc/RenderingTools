#include "bakkesmod/wrappers/canvaswrapper.h"
#include "Plane.h"
#include "Matrix3.h"
#include "Grid.h"
#include "Line.h"
#include "Frustum.h"
#include "../Extra/WrapperStructsExtensions.h"

RT::Plane::Plane()
	: x(0), y(0), z(0), d(0) {}

RT::Plane::Plane(float X, float Y, float Z, float D)
	: x(X), y(Y), z(Z), d(D) {}

RT::Plane::Plane(Vector normal, float distance)
	: x(normal.X), y(normal.Y), z(normal.Z), d(distance) {}

RT::Plane::Plane(Vector normal, Vector location)
{
	normal.normalize();
	float distance = Vector::dot((normal * -1), location);
	
	x = normal.X;
	y = normal.Y;
	z = normal.Z;
	d = distance;
}

void RT::Plane::Draw(CanvasWrapper canvas, Frustum &frustum, float size, int squares) const
{
	Matrix3 planeMat;
	planeMat.forward = direction();
	if(Vector::dot(direction(), Vector(0,0,1)) != 1.0)
	{
		planeMat.right = Vector::cross(planeMat.forward, Vector(0,0,1));
		planeMat.up = Vector::cross(planeMat.forward, planeMat.right);
	}
	else
	{
		//Plane is flat on ground, cross products won't work
		planeMat.right = Vector(1,0,0);
		planeMat.up = Vector(0,1,0);
	}
	planeMat.normalize();

	//-d here might actually need to be d. Needs further testing
	Grid grid(direction() * -d, planeMat.ToQuat(), size, size, squares, squares);
	grid.Draw(canvas, frustum);
	DrawVector(canvas, direction(), direction() * -d, size / 2);
}

Vector RT::Plane::direction() const
{
	Vector dir = Vector{x, y, z};
	dir.normalize();
	
	return dir;
}

bool RT::Plane::LineIntersectsWithPlane(Line &line) const
{
	//if the line is parallel to the plane, it will not intersect
	if(Vector::dot(line.direction(), direction()) != 0)
		return true;
	else
		return false;
}

Vector RT::Plane::LinePlaneIntersectionPoint(Line &line) const
{
	//f = plane (4D in numerator, 3D in denominator)
	//p = lineStart (with additional 4D value of 1 in numerator)
	//v = direction (3D)
	//Vector q = p - (dot(f4D,p4D)/dot(f3D,v)) * v

	float dot4D = x * line.lineBegin.X   + y * line.lineBegin.Y   + z * line.lineBegin.Z + d;
	float dot3D = x * line.direction().X + y * line.direction().Y + z * line.direction().Z;
	float t = dot4D / dot3D;

	return (line.lineBegin - line.direction() * t);
}
