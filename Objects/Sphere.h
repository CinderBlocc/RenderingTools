#pragma once
#include "bakkesmod/wrappers/wrapperstructs.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "Line.h"

namespace RT
{
	class Frustum;

	class Sphere
	{
	public:
		Vector location;
		Quat orientation;
		float radius;

		// CONSTRUCTORS
		explicit Sphere();
		explicit Sphere(Vector loc, float rad);
		explicit Sphere(Vector loc, Quat rot, float rad);

		// FUNCTIONS
		void Draw(CanvasWrapper canvas, Frustum &frustum, Vector cameraLocation, int segments);

		bool IsOccludingLine(Line line);
	};
}
