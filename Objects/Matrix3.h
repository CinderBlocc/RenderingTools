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

		/* CONSTRUCTORS */
		Matrix3()
			: forward(Vector{1,0,0}), right(Vector{0,1,0}), up(Vector{0,0,1}) {}
		Matrix3(Vector f, Vector r, Vector u)
			: forward(f), right(r), up(u) {}
		Matrix3(Quat q)
			{ QuatToMatrix(q); }
		Matrix3(Rotator rot)
			{ RotatorToMatrix(rot); }

		/* FUNCTIONS */
		void Draw(CanvasWrapper canvas, Vector location, float size=100);
		
		Quat    ToQuat();
		Rotator ToRotator();
		Matrix3 QuatToMatrix(Quat q);
		Matrix3 RotatorToMatrix(Rotator rot);

		Matrix3 RotateWithQuat(Quat q, bool normalize=false);

		void normalize();
		const static Matrix3 identity();
	};
}
