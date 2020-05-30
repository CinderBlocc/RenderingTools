#pragma once
#include "bakkesmod/wrappers/wrapperstructs.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "Plane.h"

namespace RT
{
	class Triangle
	{
	public:
		Vector vert1;
		Vector vert2;
		Vector vert3;

		/* CONSTRUCTORS */
		Triangle()
			: vert1(Vector{0,0,0}), vert2(Vector{0,0,0}), vert3(Vector{0,0,0}) {}
		Triangle(Vector v1, Vector v2, Vector v3)
			: vert1(v1), vert2(v2), vert3(v3) {}

		/* FUNCTIONS */
		//void Draw(CanvasWrapper canvas);

		Plane GetPlaneFromTriangle();

		bool LineTriangleIntersection(Line line);
		bool IsWithinTriangleCoordinates(Vector point);
	};
}
