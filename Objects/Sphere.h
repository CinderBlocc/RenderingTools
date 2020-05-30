#pragma once
#include "bakkesmod/wrappers/wrapperstructs.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "Line.h"

namespace RT
{
	class Frustum;

	class Sphere
	{
	public:
		Vector location;
		Quat orientation;
		float radius;

		/* CONSTRUCTORS */
		Sphere()
			: location(Vector(0,0,0)), orientation(Quat(1,0,0,0)), radius(100) {}
		Sphere(Vector loc, float rad)
			: location(loc), orientation(Quat()), radius(rad) {}
		Sphere(Vector loc, Quat rot, float rad)
			: location(loc), orientation(rot), radius(rad) {}

		/* FUNCTIONS */
		void Draw(CanvasWrapper canvas, Frustum &frustum, Vector cameraLocation, int segments);

		bool IsOccludingLine(Line line);
	};
}
