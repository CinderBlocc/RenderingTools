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

		// CONSTRUCTORS
		explicit Circle();
		explicit Circle(Vector loc, Quat rot, float rad);

		// FUNCTIONS
		void Draw(CanvasWrapper canvas, Frustum &frustum);
		void DrawSegmented(CanvasWrapper canvas, Frustum &frustum, int segments, float percentPerSeg=0.5f);
	};
}
