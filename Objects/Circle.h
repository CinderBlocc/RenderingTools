#pragma once
#include "bakkesmod/wrappers/wrapperstructs.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"

namespace RT
{
	class Frustum;

	class Circle
	{
	public:
		Vector location;
		Quat orientation;
		float radius;
		float lineThickness;
		float piePercentage;
		int steps;

		/* CONSTRUCTORS */
		Circle()
			: location(Vector(0,0,0)), orientation(Quat(1,0,0,0)), radius(100), lineThickness(1), piePercentage(1), steps(16) {}
		Circle(Vector loc, Quat rot, float rad)
			: Circle() { location = loc; orientation = rot; radius = rad; }

		/* FUNCTIONS */
		void Draw(CanvasWrapper canvas, Frustum &frustum);
		void DrawSegmented(CanvasWrapper canvas, Frustum &frustum, int segments, float percentPerSeg=0.5f);
	};
}
