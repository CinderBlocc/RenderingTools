#pragma once
#include "bakkesmod/wrappers/wrapperstructs.h"

class CanvasWrapper;

namespace RT
{
	class Frustum;
    class Plane;
    class Line;

	class Triangle
	{
	public:
		Vector vert1;
		Vector vert2;
		Vector vert3;

		// CONSTRUCTORS
		explicit Triangle();
		explicit Triangle(Vector v1, Vector v2, Vector v3);

		// FUNCTIONS
		void Draw(CanvasWrapper canvas) const;
		//void DrawWithinFrustum(CanvasWrapper canvas, Frustum &frustum) const; // Future implementation: constrain triangle to clip space
		void DrawOutline(CanvasWrapper canvas, Frustum &frustum, float lineThickness = 1.0f, bool drawNormal=false) const;

		Plane GetPlaneFromTriangle() const;

		bool LineTriangleIntersection(Line &line) const;
		bool IsWithinTriangleCoordinates(Vector point) const;
	};
}
