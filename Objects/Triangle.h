#pragma once
#include "bakkesmod/wrappers/wrapperstructs.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "Plane.h"

namespace RT
{
	class Frustum;

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
		void Draw(CanvasWrapper canvas);
		//void DrawWithinFrustum(CanvasWrapper canvas, Frustum &frustum); // Future implementation: constrain triangle to clip space
		void DrawOutline(CanvasWrapper canvas, Frustum &frustum, float lineThickness=1, bool drawNormal=false);

		Plane GetPlaneFromTriangle();

		bool LineTriangleIntersection(Line line);
		bool IsWithinTriangleCoordinates(Vector point);
	};
}
