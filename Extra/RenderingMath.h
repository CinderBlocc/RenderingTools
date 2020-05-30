#pragma once
#include "bakkesmod/wrappers/wrapperstructs.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "../Objects/Matrix3.h"

enum class LookAtAxis
{
	AXIS_FORWARD = 1,
	AXIS_RIGHT = 2,
	AXIS_UP = 3
};

namespace RT
{
	RT::Matrix3 LookAt(Vector objectLocation, Vector lookAtLocation, LookAtAxis axis, float rollAmount=0);
	Quat AngleAxisRotation(float angle, Vector axis);
}
