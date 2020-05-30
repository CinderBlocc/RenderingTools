#include "RenderingTools.h"
#define _USE_MATH_DEFINES
#include <math.h>

/*

	REDO ALL OF THIS IN A CLASS BASED STYLE
		- Have an overarching assistant object that contains commonly used objects like the frustum so you dont recalculate it multiple times per frame

		- Once everything has totally split off, only a few things should remain here, such as:
			- Everything under the MISCELLANEOUS CANVAS FUNCTIONS comment
*/

//Rotations
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
	double a = (targetDirectionRejected - finalAxis).magnitude();
	double b = targetDirectionRejected.magnitude();
	double c = finalAxis.magnitude();
	float rotAngle = acos((b*b + c*c - a*a)/2*b*c);

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

//Miscellaneous canvas functions
LinearColor RT::GetPercentageColor(float percent, float alpha)
{
	LinearColor color = {0, 0, 0, 255*alpha};
	if(percent <= .5f)
	{
		color.R = 255;
		color.G = 0 + (255 * (percent*2));
	}
	else
	{
		color.R = 255 - (255 * (percent*2));
		color.G = 255;
	}

	return color;
}
float RT::GetVisualDistance(CanvasWrapper canvas, Frustum frustum, CameraWrapper camera, Vector objectLocation)
{
	//Concept: project a line of set 3D length to the 2D canvas.
	//Calculate the length of that 2D line to get perceived distance instead of linear distance

	Quat camQuat = RotatorToQuat(camera.GetRotation());
	Vector camUp = Matrix3(camQuat).up;

	float testScalePerspectiveLineLength = 100;
	Vector2F perspScaleLineStartProjected = canvas.ProjectF(objectLocation);
	Vector2F perspScaleLineEndProjected;
	Vector testScalePerspective = objectLocation + (camUp * testScalePerspectiveLineLength);

	//check if scale projection is within the top plane of the frustum, if not then invert the scale test line
	Vector planeNormal = {frustum.planes[0].x, frustum.planes[0].y, frustum.planes[0].z}; 
	if(Vector::dot(testScalePerspective, planeNormal) + frustum.planes[0].d + 1.0f > 0)
		perspScaleLineEndProjected = canvas.ProjectF(objectLocation + (camUp * testScalePerspectiveLineLength));
	else
		perspScaleLineEndProjected = canvas.ProjectF(objectLocation - (camUp * testScalePerspectiveLineLength));

	Vector2F perspScaleLine = {perspScaleLineEndProjected.X - perspScaleLineStartProjected.X, perspScaleLineEndProjected.Y - perspScaleLineStartProjected.Y};
	float perspScale = sqrt((double)perspScaleLine.X * perspScaleLine.X + (double)perspScaleLine.Y * perspScaleLine.Y);
	//if(perspScale > 100)
	//	perspScale = 100;
	float distancePercentage = perspScale/100;//1 is close, 0 is infinitely far away
	//if(distancePercentage > 1)
	//	distancePercentage = 1;
	if(distancePercentage < 0)
		distancePercentage = 0;

	return distancePercentage;
}
void RT::SetColor(CanvasWrapper canvas, LinearColor color)
{
	canvas.SetColor((char)color.R, (char)color.G, (char)color.B, (char)color.A);
}
void RT::SetColor(CanvasWrapper canvas, string colorName, float opacity)
{
	LinearColor color = {0,0,0,opacity};

	if(colorName == "black")	color = LinearColor{0,0,0,opacity};
	if(colorName == "white")	color = LinearColor{255,255,255,opacity};
	if(colorName == "red")		color = LinearColor{255,0,0,opacity};
	if(colorName == "green")	color = LinearColor{0,255,0,opacity};
	if(colorName == "blue")		color = LinearColor{0,0,255,opacity};
	if(colorName == "yellow")	color = LinearColor{255,255,0,opacity};
	if(colorName == "cyan")		color = LinearColor{0,255,255,opacity};

	SetColor(canvas, color);
}
void RT::DrawDebugStrings(CanvasWrapper canvas, vector<string> drawString, bool background, int width)
{
	if(drawString.empty()) return;

	Vector2 base = {50,50};
	if(background)
	{
		SetColor(canvas, "black", 150);
		canvas.SetPosition(base.minus({10,10}));
		canvas.FillBox(Vector2{width, (int)(20 * (drawString.size() + 1))});
	}

	SetColor(canvas, "green");
	for(auto it = 0; it != drawString.size(); ++it)
	{
		canvas.SetPosition(base);
		canvas.DrawString(drawString[it]);
		base.Y += 20;
	}
}
