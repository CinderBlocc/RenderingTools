#pragma once
#include "bakkesmod/wrappers/wrapperstructs.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"

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
		int widthSegs;
		int heightSegs;

		/* CONSTRUCTORS */
		Grid()
			: width(100), height(100), widthSegs(10), heightSegs(10) {}
		Grid(Vector loc, Quat rot, float w, float h, int wSegs, int hSegs)
			: location(loc), orientation(rot), width(w), height(h), widthSegs(wSegs), heightSegs(hSegs) {}

		/* FUNCTIONS */
		void Draw(CanvasWrapper canvas, Frustum &frustum, bool useThickMidline=true);
	};
}
