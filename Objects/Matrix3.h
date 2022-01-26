#pragma once
#include "bakkesmod/wrappers/wrapperstructs.h"

class CanvasWrapper;

namespace RT
{
	class Matrix3
	{
	public:
		Vector forward;
		Vector right;
		Vector up;

		// CONSTRUCTORS
		explicit Matrix3();
		explicit Matrix3(Vector f, Vector r, Vector u);
		explicit Matrix3(Quat q);
		explicit Matrix3(Rotator rot);

		// FUNCTIONS
		void Draw(CanvasWrapper canvas, Vector location, float size = 100.0f) const;
		
		Quat ToQuat() const;
		Rotator ToRotator() const;

		Matrix3 RotateWithQuat(Quat q, bool shouldNormalize=false);

		void normalize();
		const static Matrix3 identity();
	};
}
