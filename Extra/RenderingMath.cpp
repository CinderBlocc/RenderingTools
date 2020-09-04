#include "RenderingMath.h"
#include "WrapperStructsExtensions.h"
#define _USE_MATH_DEFINES
#include <math.h>

RT::Matrix3 RT::LookAt(Vector objectLocation, Vector lookAtLocation, LookAtAxis axis, float rollAmount, Vector worldUpAxis)
{
	//rollAmount should be in radians
    //worldUp is for cases where the up axis for the cross product should be different from the real world up axis

	Vector forward = lookAtLocation - objectLocation;
	forward.normalize();

	Vector right = Vector::cross(worldUpAxis, forward);
	if(right.magnitude() == 0)
	{
		//Forward axis is parallel to world Z axis
		Matrix3 defaultMat;
		return defaultMat;
	}
	right.normalize();

	Vector up = Vector::cross(forward, right);
	up.normalize();

	if(rollAmount != 0)
	{
		Quat rot = AngleAxisRotation(rollAmount, forward);
		right = RotateVectorWithQuat(right, rot, true);
		up = RotateVectorWithQuat(up, rot, true);
	}

	Matrix3 mat;

	if(axis == LookAtAxis::AXIS_RIGHT)
	{
		//Right
		mat.forward = right;
		mat.right = forward * -1;
		mat.up = up;

		Quat rot = AngleAxisRotation(static_cast<float>(M_PI), up);
		mat.RotateWithQuat(rot, true);
	}
	else if(axis == LookAtAxis::AXIS_UP)
	{
		//Up
		mat.forward = up;
		mat.right = right * -1;
		mat.up = forward;
	}
	else
	{
		//Forward
		mat.forward = forward;
		mat.right = right;
		mat.up = up;
	}

	return mat;
}

Quat RT::AngleAxisRotation(float angle, Vector axis)
{
	//Angle in radians
	Quat result;
	float angDiv2 = angle * 0.5f;
	result.W = cosf(angDiv2);
    result.X = axis.X * sinf(angDiv2);
    result.Y = axis.Y * sinf(angDiv2);
    result.Z = axis.Z * sinf(angDiv2);

	return result;
}
