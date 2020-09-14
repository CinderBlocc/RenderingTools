#include "RenderingMath.h"
#include "WrapperStructsExtensions.h"
#include "../Objects/Matrix3.h"

RT::Matrix3 RT::LookAt(Vector baseLocation, Vector targetLocation, LookAtAxis axis, float rollAmount, Vector worldUpAxis)
{
	//rollAmount should be in radians
    //worldUp is for cases where the up axis for the cross product should be different from the real world up axis

	Vector forward = targetLocation - baseLocation;
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
		right = RotateVectorWithQuat(right, rot); right.normalize();
		up = RotateVectorWithQuat(up, rot); up.normalize();
	}

	Matrix3 mat;

	if(axis == LookAtAxis::AXIS_RIGHT)
	{
		//Right
		mat.forward = right;
		mat.right = forward * -1;
		mat.up = up;

		Quat rot = AngleAxisRotation(CONST_PI_F, up);
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

RT::Matrix3 RT::SingleAxisAlignment(Matrix3 matrix, Vector targetDirection, LookAtAxis axis, int step)
{
	//Rotate matrix along one axis to align with a vector
	//Step is useful for LookAt function

	Matrix3 rotOrder = GetRotationOrder(matrix, axis, step);
	Vector primaryRotationAxis = rotOrder.forward;
	Vector secondaryRotationAxis = rotOrder.right;
	Vector finalAxis = rotOrder.up;

	Vector targetDirectionRejected = VectorRejection(targetDirection, primaryRotationAxis);
	targetDirectionRejected.normalize();
	float a = (targetDirectionRejected - finalAxis).magnitude();
	float b = targetDirectionRejected.magnitude();
	float c = finalAxis.magnitude();
	float rotAngle = acosf((b*b + c*c - a*a)/2*b*c);

	if(ShouldNegateAngle(Vector::dot(targetDirectionRejected, secondaryRotationAxis), step))
		rotAngle *= -1;

	Quat rotAngleQuat = AngleAxisRotation(rotAngle, primaryRotationAxis);
	return matrix.RotateWithQuat(rotAngleQuat, true);
}

RT::Matrix3 RT::GetRotationOrder(Matrix3 inMatrix, LookAtAxis axis, int step)
{
	//Returning as a matrix is a bit janky, but I think it's cleaner than returning vector<Vector>
	Vector firstRotationAxis;
	Vector secondRotationAxis;
	Vector finalAxis;
	if(axis == LookAtAxis::AXIS_FORWARD)
	{
		finalAxis = inMatrix.forward;
		if(step == 1)
		{
			firstRotationAxis = inMatrix.up;
			secondRotationAxis = inMatrix.right;
		}
		else if(step == 2)
		{
			firstRotationAxis = inMatrix.right;
			secondRotationAxis = inMatrix.up;
		}
	}
	else if(axis == LookAtAxis::AXIS_RIGHT)
	{
		finalAxis = inMatrix.right;
		if(step == 1)
		{
			firstRotationAxis = inMatrix.forward;
			secondRotationAxis = inMatrix.up;
		}
		else if(step == 2)
		{
			firstRotationAxis = inMatrix.up;
			secondRotationAxis = inMatrix.forward;
		}
	}
	else if(axis == LookAtAxis::AXIS_UP)
	{
		finalAxis = inMatrix.up;
		if(step == 1)
		{
			firstRotationAxis = inMatrix.forward;
			secondRotationAxis = inMatrix.right;
		}
		else if(step == 2)
		{
			firstRotationAxis = inMatrix.right;
			secondRotationAxis = inMatrix.forward;
		}
	}

	Matrix3 output;
	output.forward = firstRotationAxis;
	output.right = secondRotationAxis;
	output.up = finalAxis;
	return output;
}

bool RT::ShouldNegateAngle(float dot, int step)
{
	//Might need to flesh this out more based on which axis is being used
	if(step == 1)
	{
		if(dot <= 0)
			return true;
	}
	if(step == 2)
	{
		if(dot >= 0)
			return true;
	}

	return false;
}
