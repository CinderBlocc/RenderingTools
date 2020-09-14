#include "bakkesmod/wrappers/canvaswrapper.h"
#include "Matrix3.h"
#include "../Extra/WrapperStructsExtensions.h"

RT::Matrix3::Matrix3()
	: forward(Vector{1,0,0}), right(Vector{0,1,0}), up(Vector{0,0,1}) {}

RT::Matrix3::Matrix3(Vector f, Vector r, Vector u)
	: forward(f), right(r), up(u) {}

RT::Matrix3::Matrix3(Quat q)
{
    forward = RotateVectorWithQuat(Vector(1, 0, 0), q);
	right   = RotateVectorWithQuat(Vector(0, 1, 0), q);
	up      = RotateVectorWithQuat(Vector(0, 0, 1), q);
	normalize();
}

RT::Matrix3::Matrix3(Rotator rot)
{
    Quat q = RotatorToQuat(rot);
	*this = Matrix3(q);
}

Quat RT::Matrix3::ToQuat() const
{
	//https://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/

	Quat q;

	float trace = forward.X + right.Y + up.Z;
	if( trace > 0 )
	{
		float s = 0.5f / sqrtf(trace+ 1.0f);
		q.W =   0.25f / s;
		q.X = ( right.Z   - up.Y      ) * s;
		q.Y = ( up.X      - forward.Z ) * s;
		q.Z = ( forward.Y - right.X   ) * s;
	}
	else
	{
		if ( forward.X > right.Y && forward.X > up.Z )
		{
			float s = 2.0f * sqrtf( 1.0f + forward.X - right.Y - up.Z);
			q.W = ( right.Z - up.Y      ) / s;
			q.X =   0.25f * s;
			q.Y = ( right.X + forward.Y ) / s;
			q.Z = ( up.X    + forward.Z ) / s;
		}
		else if (right.Y > up.Z)
		{
			float s = 2.0f * sqrtf( 1.0f + right.Y - forward.X - up.Z);
			q.W = ( up.X    - forward.Z ) / s;
			q.X = ( right.X + forward.Y ) / s;
			q.Y =   0.25f * s;
			q.Z = ( up.Y    + right.Z   ) / s;
		}
		else
		{
			float s = 2.0f * sqrtf( 1.0f + up.Z - forward.X - right.Y );
			q.W = ( forward.Y - right.X   ) / s;
			q.X = ( up.X      + forward.Z ) / s;
			q.Y = ( up.Y      + right.Z   ) / s;
			q.Z =   0.25f * s;
		}
	}

	return q;
}

Rotator RT::Matrix3::ToRotator() const
{
	Quat q = ToQuat();
	return QuatToRotator(q);
}

RT::Matrix3 RT::Matrix3::RotateWithQuat(Quat q, bool shouldNormalize)
{
	forward = RotateVectorWithQuat(forward, q);
	right = RotateVectorWithQuat(right, q);
	up = RotateVectorWithQuat(up, q);

	if(shouldNormalize)
	{
		normalize();
	}

	return *this;
}

void RT::Matrix3::Draw(CanvasWrapper canvas, Vector location, float size) const
{
	LinearColor inColor = canvas.GetColor();

	//Forward
    canvas.SetColor(LinearColor{255,0,0,255});
	DrawVector(canvas, forward, location, size);

	//Right
    canvas.SetColor(LinearColor{0,255,0,255});
	DrawVector(canvas, right, location, size);

	//Up
    canvas.SetColor(LinearColor{0,0,255,255});
	DrawVector(canvas, up, location, size);

	//Root
	Vector2 root = canvas.Project(location);
	float boxSizePercent = size/100;
	if(boxSizePercent > 1) boxSizePercent = 1;
	int boxSize = static_cast<int>(10.f * boxSizePercent);

    canvas.SetColor(LinearColor{255,255,255,255});
	canvas.SetPosition(root.minus({boxSize/2,boxSize/2}));
	canvas.FillBox(Vector2{boxSize,boxSize});

	canvas.SetColor(inColor);
}

void RT::Matrix3::normalize()
{
	forward.normalize();
	right.normalize();
	up.normalize();
}

const RT::Matrix3 RT::Matrix3::identity()
{
	return Matrix3();
}
