#pragma once
#include "bakkesmod/wrappers/wrapperstructs.h"

class CanvasWrapper;

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
		explicit Plane(Vector normal, float distance = 0.0f);
		explicit Plane(Vector normal, Vector location);

		// FUNCTIONS
		void Draw(CanvasWrapper canvas, Frustum &frustum, float size = 300.0f, int32_t squares = 20) const;

		Vector direction() const;

		bool LineIntersectsWithPlane(Line &line) const;
		Vector LinePlaneIntersectionPoint(Line &line) const;
	};
}
