#pragma once
#include "bakkesmod/wrappers/wrapperstructs.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"

namespace RT
{
	//CONVERSIONS
	Rotator QuatToRotator(Quat quat);
	Quat RotatorToQuat(Rotator rot);

	//QUAT
	Quat Slerp(Quat q1, Quat q2, float percent);
	Quat NormalizeQuat(Quat q);

	//VECTOR
	Vector RotateVectorWithQuat(Vector v, Quat q, bool normalize=false);
	Vector VectorProjection(Vector vec1, Vector vec2);
	Vector VectorRejection(Vector vec1, Vector vec2);
	Vector VectorReflection(Vector incident, Vector normal);

	void DrawVector(CanvasWrapper canvas, Vector direction, Vector startLocation, float size=1);
}
