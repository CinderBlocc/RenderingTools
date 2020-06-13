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

		// CONSTRUCTORS
		explicit Cube();
		explicit Cube(Vector loc, Quat rot, float sLength);

		// FUNCTIONS
		void Draw(CanvasWrapper canvas);
	};
}
