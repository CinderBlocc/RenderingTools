#pragma once
#include "bakkesmod/wrappers/wrapperstructs.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"

namespace RT
{
	class Cone
	{
	public:
		Vector location;
		Vector direction;
		float radius;
		float height;
		float rollAmount;
		float thickness;
		int segments;

		/* CONSTRUCTORS */
		Cone()
			: location(Vector()), direction(Vector(0,0,1)), radius(5), height(20), rollAmount(0), segments(8), thickness(1) {}
		Cone(Vector loc, Vector dir)
			: Cone() { location = loc; direction = dir; }

		/* FUNCTIONS */
		void Draw(CanvasWrapper canvas);
	};
}
