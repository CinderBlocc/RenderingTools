#pragma once
#include "bakkesmod/wrappers/wrapperstructs.h"

class CanvasWrapper;

namespace RT
{
	class Frustum;
    class Line;

	class Cylinder
	{
	public:
		Vector location;
		Quat orientation;
		float radius;
		float height;
		float lineThickness;

		// CONSTRUCTORS
		explicit Cylinder();
		explicit Cylinder(float rad, float h);
		explicit Cylinder(Vector loc, float rad, float h);
		explicit Cylinder(Vector loc, Quat rot, float rad, float h);

		// FUNCTIONS
		void Draw(CanvasWrapper canvas, Frustum &frustum, int segments=16) const;
		bool IsInCylinder(Vector location) const;
        bool LineCrossesCylinder(const Line &line) const;
	};
}