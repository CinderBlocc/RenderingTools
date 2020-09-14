#include "bakkesmod/wrappers/canvaswrapper.h"
#include "Triangle.h"
#include "Line.h"
#include "Frustum.h"
#include "Plane.h"
#include "../Extra/WrapperStructsExtensions.h"

RT::Triangle::Triangle()
	: vert1(Vector{0,0,0}), vert2(Vector{0,0,0}), vert3(Vector{0,0,0}) {}

RT::Triangle::Triangle(Vector v1, Vector v2, Vector v3)
	: vert1(v1), vert2(v2), vert3(v3) {}

void RT::Triangle::Draw(CanvasWrapper canvas) const
{
	Vector2F v1F = canvas.ProjectF(vert1);
	Vector2F v2F = canvas.ProjectF(vert2);
	Vector2F v3F = canvas.ProjectF(vert3);
	
	canvas.FillTriangle(v1F, v2F, v3F);
}

// Future implementation: constrain triangle to clip space
//void RT::Triangle::DrawWithinFrustum(CanvasWrapper canvas, Frustum &frustum)
//{
//
//}

void RT::Triangle::DrawOutline(CanvasWrapper canvas, Frustum &frustum, float lineThickness, bool drawNormal) const
{
	Line(vert1, vert2, lineThickness).DrawWithinFrustum(canvas, frustum);
	Line(vert2, vert3, lineThickness).DrawWithinFrustum(canvas, frustum);
	Line(vert3, vert1, lineThickness).DrawWithinFrustum(canvas, frustum);

	if(drawNormal)
	{
		Vector normal = GetPlaneFromTriangle().direction();
		Vector midpoint = (((vert1 + vert2) * .5f) + vert3) * .5f;
		if(frustum.IsInFrustum(midpoint))
		{
			DrawVector(canvas, normal, midpoint, 100);
		}
	}
}

RT::Plane RT::Triangle::GetPlaneFromTriangle() const
{
	//Get the normal vector from the cross product of two edges of the triangle.
	//Get 'd' from negative normal dotted with known point on plane

	Vector normal = Vector::cross((vert1 - vert2), (vert3 - vert2));
	float d = Vector::dot(Vector{-normal.X, -normal.Y, -normal.Z}, vert2);

	float mag = normal.magnitude();
	Plane newPlane(normal.X/mag, normal.Y/mag, normal.Z/mag, d/mag);
	return newPlane;
}

bool RT::Triangle::LineTriangleIntersection(Line &line) const
{
	//Check if line crosses through plane
	//If it does, check if the intersection point is within the line segment
	//If it is, check if the intersection point is within the barycentric coordinates of the triangle
	Plane plane = GetPlaneFromTriangle();
	if(plane.LineIntersectsWithPlane(line))
	{
		Vector intersection = plane.LinePlaneIntersectionPoint(line);
		float intersectionSegmentLength = (intersection - line.lineBegin).magnitude();
		float lineSegmentLength = (line.lineEnd - line.lineBegin).magnitude();
		if(intersectionSegmentLength > lineSegmentLength)
			return false;
		else
		{
			if(IsWithinTriangleCoordinates(intersection))
				return true;
			else
				return false;
		}
	}
	else
		return false;

	return true;
}

bool RT::Triangle::IsWithinTriangleCoordinates(Vector point) const
{
	//Found here: https://github.com/SebLague/Gamedev-Maths/blob/master/PointInTriangle.cs video: https://youtu.be/HYAgJN3x4GA
	//and here: https://github.com/1robertslattery/GeometricTestLibrary/blob/master/Source/GeometricTests/GeometricTestLibrary.h#L1607 video: https://youtu.be/bd_JwXYVc6c

	Vector A = vert1;
	Vector B = vert2;
	Vector C = vert3;

	Plane plane = GetPlaneFromTriangle();

	float w1 = -1;
	float w2 = -1;

	if(fabs(plane.x) >= fabs(plane.y) && fabs(plane.x) >= fabs(plane.z))
	{
		//discard X
		float s1 = C.Y - A.Y;
		float s2 = C.Z - A.Z;
		float s3 = B.Y - A.Y;
		float s4 = point.Y - A.Y;

		w1 = (A.Z*s1 + s4*s2 - point.Z*s1) / (s3*s2 - (B.Z - A.Z)*s1);
		w2 = (s4 - w1*s3) / s1;
	}
	else if(fabs(plane.y) >= fabs(plane.z))
	{
		//discard Y
		float s1 = C.Z - A.Z;
		float s2 = C.X - A.X;
		float s3 = B.Z - A.Z;
		float s4 = point.Z - A.Z;

		w1 = (A.X*s1 + s4*s2 - point.X*s1) / (s3*s2 - (B.X - A.X)*s1);
		w2 = (s4 - w1*s3) / s1;
	}
	else
	{
		//discard Z
		float s1 = C.Y - A.Y;
		float s2 = C.X - A.X;
		float s3 = B.Y - A.Y;
		float s4 = point.Y - A.Y;

		w1 = (A.X*s1 + s4*s2 - point.X*s1) / (s3*s2 - (B.X - A.X)*s1);
		w2 = (s4 - w1*s3) / s1;
	}
	
	return w1 >= 0 && w2 >= 0 && (w1 + w2) <= 1;
}
