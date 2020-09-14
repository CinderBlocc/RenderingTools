#pragma once
#include "bakkesmod/wrappers/wrapperstructs.h"

class CanvasWrapper;

namespace RT
{
	class Cone
	{
	public:
		Vector location;
		Vector direction;
		float radius;
		float height;
		float rollAmount;
		float thickness;
		int segments;

		// CONSTRUCTORS
		explicit Cone();
		explicit Cone(Vector loc, Vector dir);

		// FUNCTIONS
		void Draw(CanvasWrapper canvas) const;
	};
}
