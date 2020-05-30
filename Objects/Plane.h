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

		/* CONSTRUCTORS */
		Plane()
			: x(0), y(0), z(0), d(0) {}
		Plane(float X, float Y, float Z, float D)
			: x(X), y(Y), z(Z), d(D) {}
		Plane(Vector normal)
			: x(normal.X), y(normal.Y), z(normal.Z), d(0) {}
		Plane(Vector normal, float distance)
			: x(normal.X), y(normal.Y), z(normal.Z), d(distance) {}
		Plane(Vector normal, Vector location); //Defined in cpp. Creates a plane with specified normal passing through specified location

		/* FUNCTIONS */
		void Draw(CanvasWrapper canvas, Frustum &frustum, float size=300, int squares=20);

		Vector direction();

		bool LineIntersectsWithPlane(Line &line);
		Vector LinePlaneIntersectionPoint(Line &line);
	};
}
