#pragma once
#include "bakkesmod/wrappers/wrapperstructs.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"

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
		void Draw(CanvasWrapper canvas, Vector location, float size=100);
		
		Quat ToQuat();
		Rotator ToRotator();
		Matrix3 QuatToMatrix(Quat q);
		Matrix3 RotatorToMatrix(Rotator rot);

		Matrix3 RotateWithQuat(Quat q, bool normalize=false);

		void normalize();
		const static Matrix3 identity();
	};
}
