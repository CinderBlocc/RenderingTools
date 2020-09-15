#include "bakkesmod/wrappers/canvaswrapper.h"
#include "WrapperStructsExtensions.h"
#include "../Objects/Matrix3.h"
#include "../Objects/Cone.h"
#include "../Objects/Frustum.h"
#include "../Objects/Line.h"

//  All commented code has been moved into the BakkesModSDK wrapperstructs.h file  //

//Rotator RT::QuatToRotator(Quat quat)
//{
//	//From Arator's HelperFunctions
//	RT::Matrix3 matrix(quat);
//	Vector fwd = matrix.forward;
//    Vector right = matrix.right;
//    Vector up = matrix.up;
//
//	//Pitch
//	float pitch_f = asinf(fwd.Z);
//	int PITCH = static_cast<int>((pitch_f / (CONST_PI_F / 2.f)) * 16384.f);
//
//	//Yaw
//	float hor_mag = sqrtf(fwd.X * fwd.X + fwd.Y * fwd.Y);
//	float fwd_y = asinf(fwd.Y / hor_mag);
//	if (fwd_y >= 0)
//	{
//		if (fwd.X < 0)
//			fwd_y = CONST_PI_F - fwd_y;
//	}
//	else
//	{
//		if (fwd.X < 0)
//			fwd_y = -CONST_PI_F - fwd_y;
//	}
//	int YAW = static_cast<int>((fwd_y / CONST_PI_F) * 32768.f);
//
//	//Roll
//    Vector vert = (up.Z < 0) ? Vector(0, 0, -1) : Vector(0, 0, 1);
//	Vector hor_right = Vector::cross(fwd, vert) * -1;// left-handed coordinate system
//	hor_right.normalize();
//
//	float roll_cos = Vector::dot(hor_right, right);
//	float roll_f = acosf(roll_cos);
//	float up_f = asinf(up.Z);
//	
//	if (right.Z >= 0)
//	{
//		if (up.Z >= 0)
//			roll_f = -roll_f;
//		else
//			roll_f  = -CONST_PI_F + roll_f;
//	}
//	else
//	{
//		if (up.Z < 0)
//			roll_f = CONST_PI_F - roll_f;
//	}
//	int ROLL = static_cast<int>((roll_f / CONST_PI_F) * 32768.f);
//
//	return Rotator(PITCH, YAW, ROLL);
//}
//
//Quat RT::RotatorToQuat(Rotator rot)
//{
//	float rotatorToRadian = (((CONST_PI_F / 180.f) * .5f) / 182.044449f);
//	float sinPitch = sinf(rot.Pitch * rotatorToRadian);
//	float cosPitch = cosf(rot.Pitch * rotatorToRadian);
//	float sinYaw   = sinf(rot.Yaw   * rotatorToRadian);
//	float cosYaw   = cosf(rot.Yaw   * rotatorToRadian);
//    float sinRoll  = sinf(rot.Roll  * rotatorToRadian);
//    float cosRoll  = cosf(rot.Roll  * rotatorToRadian);
//	
//	Quat convertedQuat;
//	convertedQuat.X = (cosRoll  * sinPitch * sinYaw) - (sinRoll * cosPitch * cosYaw);
//	convertedQuat.Y = (-cosRoll * sinPitch * cosYaw) - (sinRoll * cosPitch * sinYaw);
//	convertedQuat.Z = (cosRoll  * cosPitch * sinYaw) - (sinRoll * sinPitch * cosYaw);
//	convertedQuat.W = (cosRoll  * cosPitch * cosYaw) + (sinRoll * sinPitch * sinYaw);
//
//	return convertedQuat;
//}
//
//Quat RT::Slerp(Quat q1, Quat q2, float percent)
//{
//	//percent range: 0-1
//	//https://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/index.htm
//	float dot = q1.W*q2.W + q1.X*q2.X + q1.Y*q2.Y + q1.Z*q2.Z;
//	// if qa=qb or qa=-qb then theta = 0 and we can return qa
//	if (abs(dot) >= 1.f)
//	{
//		return q1;
//	}
//	// Calculate temporary values.
//	float halfTheta = acosf(dot);
//	float sinHalfTheta = sqrtf(1.f - dot*dot);
//	// if theta = 180 degrees then result is not fully defined
//	// we could rotate around any axis normal to qa or qb
//	if (fabsf(sinHalfTheta) < 0.001f)
//	{
//        Quat q;
//        q.W = q1.W * 0.5f + q2.W * 0.5f;
//        q.X = q1.X * 0.5f + q2.X * 0.5f;
//        q.Y = q1.Y * 0.5f + q2.Y * 0.5f;
//        q.Z = q1.Z * 0.5f + q2.Z * 0.5f;
//		return q;
//	}
//	float ratioA = sinf((1.f - percent) * halfTheta) / sinHalfTheta;
//	float ratioB = sinf(percent * halfTheta) / sinHalfTheta;
//
//    Quat q;
//	q.W = (q1.W * ratioA + q2.W * ratioB);
//	q.X = (q1.X * ratioA + q2.X * ratioB);
//	q.Y = (q1.Y * ratioA + q2.Y * ratioB);
//	q.Z = (q1.Z * ratioA + q2.Z * ratioB);
//	
//	return q;
//}
//
//Quat RT::NormalizeQuat(Quat q)
//{
//	float mag = sqrtf(q.X*q.X + q.Y*q.Y + q.Z*q.Z + q.W*q.W);
//	q.X /= mag;
//	q.Y /= mag;
//	q.Z /= mag;
//
//	return q;
//}
//
//Vector RT::RotateVectorWithQuat(Vector v, Quat q, bool normalize)
//{
//	//From Arator's HelperFunctions: https://github.com/AratorRL/SciencePlugin/blob/ebb74838471f9352c1684b166f0d98eaa1604d31/SciencePlugin/HelperFunctions.cpp
//	Quat p;
//	p.W = 0;
//	p.X = v.X;
//	p.Y = v.Y;
//	p.Z = v.Z;
//
//	Quat result = (q * p) * q.conjugate();
//	Vector res = {result.X, result.Y, result.Z};
//
//	if(normalize) res.normalize();
//
//	return res;
//}

Vector RT::VectorProjection(Vector vec1, Vector vec2)
{
	float dot = Vector::dot(vec1, vec2);
	float vec2magnitude = vec2.magnitude();
	return (vec2 * dot/(vec2magnitude * vec2magnitude));
}

Vector RT::VectorRejection(Vector vec1, Vector vec2)
{
	return vec1 - VectorProjection(vec1, vec2);
}

Vector RT::VectorReflection(Vector incident, Vector normal)
{
	//From JsonV
	//taken from: https://en.wikipedia.org/wiki/Specular_reflection
	normal.normalize();
	float norm_dot = Vector::dot(incident, normal);
	if (norm_dot < 0)
	{
		normal = normal * -1;
		norm_dot *= -1.0;
	}
	norm_dot *= 2.0;
	Vector change = normal * norm_dot;
	Vector r_unit = change - incident;
	return r_unit;
}

void RT::DrawVector(CanvasWrapper canvas, Vector direction, Vector startLocation, float size)
{
	//Draws a vector from a starting location. Uses the vector's magnitude to determine length.
	//"size" is useful for drawing normalized vectors to multiply their magnitude so that they are visible.
	//Not recommended to use size for non-normalized vectors. It is defaulted to 1.

	Vector newVec = (direction * size) + startLocation;
	float mag = (newVec - startLocation).magnitude();
	
	//If vector length is less than default cone height, shrink cone to fit inside vector
	Cone cone(newVec, direction);
	if(cone.height > mag)
	{
		float originalConeHeight = cone.height;
		cone.height = mag;

		float ratio = originalConeHeight / cone.height;
		cone.radius /= ratio;
	}

	//Subtract the cone's height from the vector so cone tip is directly on the vector instead of cone base
	direction.normalize();
	newVec = newVec - direction * cone.height;
	cone.location = newVec;

	//Draw line and cone
	canvas.DrawLine(canvas.ProjectF(startLocation), canvas.ProjectF(newVec));
	cone.Draw(canvas);
}

void RT::DrawVectorWithinFrustum(CanvasWrapper canvas, Frustum& frustum, Vector direction, Vector startLocation, float size)
{
	//Draws a vector from a starting location. Uses the vector's magnitude to determine length.
	//"size" is useful for drawing normalized vectors to multiply their magnitude so that they are visible.
	//Not recommended to use size for non-normalized vectors. It is defaulted to 1.

	Vector newVec = (direction * size) + startLocation;
	float mag = (newVec - startLocation).magnitude();
	
	//If vector length is less than default cone height, shrink cone to fit inside vector
	Cone cone(newVec, direction);
	if(cone.height > mag)
	{
		float originalConeHeight = cone.height;
		cone.height = mag;

		float ratio = originalConeHeight / cone.height;
		cone.radius /= ratio;
	}

	//Subtract the cone's height from the vector so cone tip is directly on the vector instead of cone base
	direction.normalize();
	newVec = newVec - direction * cone.height;
	cone.location = newVec;

	//Draw line and cone
    RT::Line renderLine(startLocation, newVec);
    renderLine.DrawWithinFrustum(canvas, frustum);
	
    if(frustum.IsInFrustum(cone.location, cone.radius))
    {
        cone.Draw(canvas);
    }
}
