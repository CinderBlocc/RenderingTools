#pragma once
#include "bakkesmod/wrappers/wrapperstructs.h"

class CanvasWrapper;

namespace RT
{
	class Frustum;

	class Grid
	{
	public:
		Vector location;
		Quat orientation;
		float width;
		float height;
		int32_t widthSegs;
		int32_t heightSegs;

		// CONSTRUCTORS
		explicit Grid();
		explicit Grid(Vector loc, Quat rot, float w, float h, int32_t wSegs, int32_t hSegs);

		// FUNCTIONS
		void Draw(CanvasWrapper canvas, Frustum &frustum, bool useThickMidline=true) const;
	};
}
