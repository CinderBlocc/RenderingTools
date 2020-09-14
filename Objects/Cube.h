#pragma once
#include "bakkesmod/wrappers/wrapperstructs.h"

class CanvasWrapper;

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
        explicit Cube(Vector loc);
        explicit Cube(Vector loc, float sLength);
		explicit Cube(Vector loc, Quat rot, float sLength);

		// FUNCTIONS
		void Draw(CanvasWrapper canvas) const;
	};
}
