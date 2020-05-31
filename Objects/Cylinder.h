#pragma once
#include "bakkesmod/wrappers/wrapperstructs.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"

namespace RT
{
	class Frustum;

	class Cylinder
	{
	public:
		Vector location;
		Quat orientation;
		float radius;
		float height;
		float lineThickness;

		/* CONSTRUCTORS */
		Cylinder()
			: location(Vector()), orientation(Quat()), radius(50), height(100), lineThickness(1) {}
		Cylinder(float rad, float h)
			: location(Vector()), orientation(Quat()), radius(rad), height(h), lineThickness(1) {}
		Cylinder(Vector loc, float rad, float h)
			: location(loc), orientation(Quat()), radius(rad), height(h), lineThickness(1) {}
		Cylinder(Vector loc, Quat rot, float rad, float h)
			: location(loc), orientation(rot), radius(rad), height(h), lineThickness(1) {}

		/* FUNCTIONS */
		void Draw(CanvasWrapper canvas, Frustum &frustum, int segments=16);
		bool IsInCylinder(Vector location);
	};
}