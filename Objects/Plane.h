#pragma once
#include "bakkesmod/wrappers/wrapperstructs.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"

namespace RT
{
	class Line;
	class Frustum;

	class Plane
	{
	public:
		float x;
		float y;
		float z;
		float d;

		// CONSTRUCTORS
		explicit Plane();
		explicit Plane(float X, float Y, float Z, float D);
		explicit Plane(Vector normal, float distance=0);
		explicit Plane(Vector normal, Vector location);

		// FUNCTIONS
		void Draw(CanvasWrapper canvas, Frustum &frustum, float size=300, int squares=20);

		Vector direction();

		bool LineIntersectsWithPlane(Line &line);
		Vector LinePlaneIntersectionPoint(Line &line);
	};
}
