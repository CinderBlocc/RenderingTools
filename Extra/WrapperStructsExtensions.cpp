#include "WrapperStructsExtensions.h"
#include "../Objects/Matrix3.h"
#include "../Objects/Cone.h"
#define _USE_MATH_DEFINES
#include <math.h>

Rotator RT::QuatToRotator(Quat quat)
{
	//From Arator's HelperFunctions
	RT::Matrix3 matrix(quat);
	Vector fwd = matrix.forward;
	Vector up = matrix.right;
	Vector right = matrix.up;

	//Pitch
	float pitch_f = static_cast<float>(asin(fwd.Z));
	int PITCH = static_cast<int>((pitch_f / (M_PI / 2)) * 16384);

	//Yaw
	float hor_mag = static_cast<float>(sqrt((double)fwd.X * fwd.X + (double)fwd.Y * fwd.Y));
	float hor_y = fwd.Y / hor_mag;
	float fwd_y = static_cast<float>(asin(hor_y));
	if (fwd_y >= 0)
	{
		if (fwd.X >= 0)
			fwd_y = fwd_y;
		else
			fwd_y = static_cast<float>(M_PI) - fwd_y;
	}
	else
	{
		if (fwd.X >= 0)
			fwd_y = fwd_y;
		else
			fwd_y = static_cast<float>(-M_PI) - fwd_y;
	}
	int YAW = static_cast<int>((fwd_y / M_PI) * 32768);

	//Roll
	Vector vert = Vector(0, 0, 1);
	if(up.Z < 0)
		vert = Vector(0, 0, -1);
	Vector hor_right = Vector::cross(fwd, vert);
	hor_right = { -hor_right.X, -hor_right.Y, -hor_right.Z }; // left-handed coordinate system
	hor_right.normalize();
	float roll_cos = Vector::dot(hor_right, right);
	float roll_f = static_cast<float>(acos(roll_cos));
	
	float up_f = static_cast<float>(asin(up.Z));
	
	if (right.Z >= 0)
	{
		if (up.Z >= 0)
			roll_f = -roll_f;
		else
			roll_f  = static_cast<float>(-M_PI) + roll_f;
	}
	else
	{
		if (up.Z >= 0)
			roll_f = roll_f;
		else
			roll_f = static_cast<float>(M_PI) - roll_f;
	}
	int ROLL = static_cast<int>((roll_f / M_PI) * 32768);

	return Rotator(PITCH, YAW, ROLL);
}

Quat RT::RotatorToQuat(Rotator rot)
{
	float rotatorToRadian = static_cast<float>(((M_PI / 180) / 2) / 182.044449);
	float sinPitch = static_cast<float>(sin((double)rot.Pitch*rotatorToRadian));
	float cosPitch = static_cast<float>(cos((double)rot.Pitch*rotatorToRadian));
	float sinYaw = static_cast<float>(sin((double)rot.Yaw*rotatorToRadian));
	float cosYaw = static_cast<float>(cos((double)rot.Yaw*rotatorToRadian));
    float sinRoll = static_cast<float>(sin((double)rot.Roll*rotatorToRadian));
    float cosRoll = static_cast<float>(cos((double)rot.Roll*rotatorToRadian));
	
	Quat convertedQuat;
	convertedQuat.X = (cosRoll*sinPitch*sinYaw) - (sinRoll*cosPitch*cosYaw);
	convertedQuat.Y = ((-cosRoll)*sinPitch*cosYaw) - (sinRoll*cosPitch*sinYaw);
	convertedQuat.Z = (cosRoll*cosPitch*sinYaw) - (sinRoll*sinPitch*cosYaw);
	convertedQuat.W = (cosRoll*cosPitch*cosYaw) + (sinRoll*sinPitch*sinYaw);

	return convertedQuat;
}

Quat RT::Slerp(Quat q1, Quat q2, float percent)
{
	//percent range: 0-1
	//https://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/index.htm
	Quat q;
	float dot = q1.W*q2.W + q1.X*q2.X + q1.Y*q2.Y + q1.Z*q2.Z;
	// if qa=qb or qa=-qb then theta = 0 and we can return qa
	if (abs(dot) >= 1.0)
	{
		q.W = q1.W;
		q.X = q1.X;
		q.Y = q1.Y;
		q.Z = q1.Z;
		return q;
	}
	// Calculate temporary values.
	double halfTheta = acos(dot);
	double sinHalfTheta = sqrt(1.0 - (double)dot*dot);
	// if theta = 180 degrees then result is not fully defined
	// we could rotate around any axis normal to qa or qb
	if (fabs(sinHalfTheta) < 0.001)
	{
        q.W = q1.W * 0.5f + q2.W * 0.5f;
        q.X = q1.X * 0.5f + q2.X * 0.5f;
        q.Y = q1.Y * 0.5f + q2.Y * 0.5f;
        q.Z = q1.Z * 0.5f + q2.Z * 0.5f;
		return q;
	}
	float ratioA = static_cast<float>(sin((1.0 - percent) * halfTheta) / sinHalfTheta);
	float ratioB = static_cast<float>(sin(percent * halfTheta) / sinHalfTheta); 

	q.W = (q1.W * ratioA + q2.W * ratioB);
	q.X = (q1.X * ratioA + q2.X * ratioB);
	q.Y = (q1.Y * ratioA + q2.Y * ratioB);
	q.Z = (q1.Z * ratioA + q2.Z * ratioB);
	
	return q;
}

Quat RT::NormalizeQuat(Quat q)
{
	float mag = static_cast<float>(sqrt((double)q.X*q.X + (double)q.Y*q.Y + (double)q.Z*q.Z + (double)q.W*q.W));
	q.X /= mag;
	q.Y /= mag;
	q.Z /= mag;

	return q;
}

Vector RT::RotateVectorWithQuat(Vector v, Quat q, bool normalize)
{
	//From Arator's HelperFunctions: https://github.com/AratorRL/SciencePlugin/blob/ebb74838471f9352c1684b166f0d98eaa1604d31/SciencePlugin/HelperFunctions.cpp
	Quat p;
	p.W = 0;
	p.X = v.X;
	p.Y = v.Y;
	p.Z = v.Z;

	Quat result = (q * p) * q.conjugate();
	Vector res = {result.X, result.Y, result.Z};

	if(normalize) res.normalize();

	return res;
}

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
