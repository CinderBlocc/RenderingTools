#pragma once
#include "bakkesmod/wrappers/wrapperstructs.h"

enum class LookAtAxis
{
	AXIS_FORWARD = 1,
	AXIS_RIGHT = 2,
	AXIS_UP = 3
};

namespace RT
{
    class Matrix3;

    Matrix3 LookAt(Vector baseLocation, Vector targetLocation, LookAtAxis axis, float rollAmount=0, Vector worldUpAxis={0,0,1});
	Quat AngleAxisRotation(float angle, Vector axis);
    Matrix3 SingleAxisAlignment(Matrix3 matrix, Vector targetDirection, LookAtAxis axis, int step);
	Matrix3 GetRotationOrder(Matrix3 inMatrix, LookAtAxis axis, int step);
	bool ShouldNegateAngle(float dot, int step);
}
