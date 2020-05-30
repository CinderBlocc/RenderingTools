#pragma once
#include "bakkesmod/wrappers/wrapperstructs.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"

namespace RT
{
	class Cube
	{
	public:
		Vector location;
		Quat orientation;
		float sideLength;
		float lineThickness;

		/* CONSTRUCTORS */
		Cube()
			: location(Vector()), orientation(Quat()), sideLength(50), lineThickness(1) {}
		Cube(Vector loc, Quat rot, float sLength)
			: location(loc), orientation(rot), sideLength(sLength), lineThickness(1) {}

		/* FUNCTIONS */
		void Draw(CanvasWrapper canvas);
	};
}
