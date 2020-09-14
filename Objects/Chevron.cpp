#include "bakkesmod/wrappers/canvaswrapper.h"
#include "Chevron.h"
#include "Line.h"
#include "Triangle.h"
#include "Matrix3.h"
#include "Frustum.h"
#include "../Extra/WrapperStructsExtensions.h"

RT::Chevron::Chevron()
	: location(Vector()), orientation(Quat()), length(200), width(200), thickness(100), wipeTipToTail(0), wipeTailToTip(0) 
{
	UpdateMaxThickness();
	if(thickness > maxThickness)
	{
		thickness = maxThickness;
	}
	UpdateBaseVertices();
}

RT::Chevron::Chevron(Vector loc, Quat rot)
	: Chevron()
{
	location = loc;
	orientation = rot;

	UpdateMaxThickness();
	if(thickness > maxThickness)
	{
		thickness = maxThickness;
	}
	UpdateBaseVertices();
}

RT::Chevron::Chevron(Vector loc, Quat rot, float len, float wid, float thicc, float tipToTail, float tailToTip)
	: location(loc), orientation(rot), length(len), width(wid), thickness(thicc), wipeTipToTail(tipToTail), wipeTailToTip(tailToTip)
{
	//Constrain thickness
	UpdateMaxThickness();
	if(thickness > maxThickness)
	{
		thickness = maxThickness;
	}

	//Dont let the wipe values pass each other
	if(wipeTailToTip > (1 - wipeTipToTail))
	{
		wipeTipToTail = 1 - wipeTailToTip;
	}

	UpdateBaseVertices();
}

void RT::Chevron::Draw(CanvasWrapper canvas, Frustum &frustum, bool showLines) const
{
	//Both wipe values completely obscure chevron, don't draw
	if(wipeTailToTip + wipeTipToTail >= 1)
	{
		return;
	}

	//Get orientation as a matrix
	Matrix3 chevMat(orientation);

	//Chevron is outside frustum, don't draw
	Vector middle = location + chevMat.forward * GetFullLength() * .5f;
	if(!frustum.IsInFrustum(middle, GetFullLength() * .25f))
	{
		return;
	}

	//Calculate wipe in and wipe out positions
	Line tipSplitLine(tipInnerVert, tipVert);
	Line outerLine(tipVert, outerVert);
	Line innerLine(tipInnerVert, innerVert);
	Line bottomSplitLine(bottomVert, bottomSplitVert);
	Line bottomOuterLine(bottomVert, outerVert);
	Line bottomInnerLine(bottomVert, innerVert);

	Vector newTip = tipVert;
	Vector newTipInner = tipInnerVert;
	Vector newOuter = outerVert;
	Vector newInner = innerVert;
	Vector newBottomSplit = bottomSplitVert;
	Vector newBottomOuter = bottomVert;
	Vector newBottomInner = bottomVert;

	bool drawTip = true;
	bool drawMiddle = true;
	bool drawBottom = true;

	if(wipeTipToTail > 0 && wipeTipToTail <= 1)
	{
		if(wipeTipToTail <= topPerc + middlePerc)
		{
			//Split the top of the chevron until it reaches the middle
			float outerLinePerc = wipeTipToTail / (topPerc + middlePerc);
			newTip = outerLine.GetPointAlongLine(outerLinePerc);

			if(wipeTipToTail >= topPerc)
			{
				//Push the middle verts down their respective lines
				float innerLinePerc = (wipeTipToTail - topPerc) / middlePerc;
				newTipInner = innerLine.GetPointAlongLine(innerLinePerc);

				drawTip = false;
			}
		}
		else
		{
			//Collapse bottom verts down upon themselves
			float bottomLinePerc = 1 - ((wipeTipToTail - topPerc - middlePerc) / bottomPerc);
			newOuter = bottomOuterLine.GetPointAlongLine(bottomLinePerc);
			newInner = bottomInnerLine.GetPointAlongLine(bottomLinePerc);
			newBottomSplit = bottomSplitLine.GetPointAlongLine(bottomLinePerc);

			drawTip = false;
			drawMiddle = false;
		}
	}
	if(wipeTailToTip > 0 && wipeTailToTip <= 1)
	{
		if(wipeTailToTip <= bottomPerc)
		{
			//Split the bottom of the chevron until it reaches the middle
			float bottomLinePerc = wipeTailToTip / bottomPerc;
			newBottomOuter = bottomOuterLine.GetPointAlongLine(bottomLinePerc);
			newBottomInner = bottomInnerLine.GetPointAlongLine(bottomLinePerc);
		}
		else
		{
			//Push the middle verts up their respective lines
			float outerLinePerc = (wipeTailToTip - bottomPerc) / (middlePerc + topPerc);
			float innerLinePerc = (wipeTailToTip - bottomPerc) / middlePerc;
			newOuter = outerLine.GetPointAlongLine(1 - outerLinePerc);
			newInner = innerLine.GetPointAlongLine(1 - innerLinePerc);

			drawBottom = false;
			if(wipeTailToTip >= bottomPerc + middlePerc)
			{
				//Collapse the top verts down upon themselves
				float tipLinePerc = (wipeTailToTip - bottomPerc - middlePerc) / topPerc;
				newTipInner = tipSplitLine.GetPointAlongLine(tipLinePerc);
				newInner = newTipInner;
			}
		}
	}

	//Rotate left vertices
	Vector leftTipVert          = RotateVectorWithQuat(newTip,          orientation);
	Vector leftTipInnerVert     = RotateVectorWithQuat(newTipInner,     orientation);
	Vector leftOuterVert        = RotateVectorWithQuat(newOuter,        orientation);
	Vector leftBottomOuterVert  = RotateVectorWithQuat(newBottomOuter,  orientation);
	Vector leftBottomInnerVert  = RotateVectorWithQuat(newBottomInner,  orientation);
	Vector leftInnerVert        = RotateVectorWithQuat(newInner,        orientation);
	Vector leftBottomSplitVert  = RotateVectorWithQuat(newBottomSplit,  orientation);

	//Mirror left vertices to right side
	Vector rightTipVert         = VectorReflection(leftTipVert,         chevMat.forward);
	Vector rightOuterVert       = VectorReflection(leftOuterVert,       chevMat.forward);
	Vector rightBottomOuterVert = VectorReflection(leftBottomOuterVert, chevMat.forward);
	Vector rightBottomInnerVert = VectorReflection(leftBottomInnerVert, chevMat.forward);
	Vector rightInnerVert       = VectorReflection(leftInnerVert,       chevMat.forward);
	Vector rightTipInnerVert    = VectorReflection(leftTipInnerVert,    chevMat.forward);
	Vector rightBottomSplitVert = VectorReflection(leftBottomSplitVert, chevMat.forward);

	//Translate all vertices
	leftTipVert          = leftTipVert          + location;
	leftOuterVert        = leftOuterVert        + location;
	leftBottomOuterVert  = leftBottomOuterVert  + location;
	leftBottomInnerVert  = leftBottomInnerVert  + location;
	leftInnerVert        = leftInnerVert        + location;
	leftTipInnerVert     = leftTipInnerVert     + location;
	leftBottomSplitVert  = leftBottomSplitVert  + location;
	rightTipVert         = rightTipVert         + location;
	rightOuterVert       = rightOuterVert       + location;
	rightBottomOuterVert = rightBottomOuterVert + location;
	rightBottomInnerVert = rightBottomInnerVert + location;
	rightInnerVert       = rightInnerVert       + location;
	rightTipInnerVert    = rightTipInnerVert    + location;
	rightBottomSplitVert = rightBottomSplitVert + location;

	//Project all vertices
	Vector2F leftTipV2F          = canvas.ProjectF(leftTipVert);
	Vector2F leftOuterV2F        = canvas.ProjectF(leftOuterVert);   
	Vector2F leftBottomOuterV2F  = canvas.ProjectF(leftBottomOuterVert);
	Vector2F leftBottomInnerV2F  = canvas.ProjectF(leftBottomInnerVert);
	Vector2F leftInnerV2F        = canvas.ProjectF(leftInnerVert);
	Vector2F leftTipInnerV2F     = canvas.ProjectF(leftTipInnerVert);
	Vector2F leftBottomSplitV2F  = canvas.ProjectF(leftBottomSplitVert);
	Vector2F rightTipV2F         = canvas.ProjectF(rightTipVert);
	Vector2F rightOuterV2F       = canvas.ProjectF(rightOuterVert);
	Vector2F rightBottomOuterV2F = canvas.ProjectF(rightBottomOuterVert);
	Vector2F rightBottomInnerV2F = canvas.ProjectF(rightBottomInnerVert);
	Vector2F rightInnerV2F       = canvas.ProjectF(rightInnerVert);
	Vector2F rightTipInnerV2F    = canvas.ProjectF(rightTipInnerVert);
	Vector2F rightBottomSplitV2F = canvas.ProjectF(rightBottomSplitVert);

	/* DRAW TRIANGLES */
	if(drawTip)
	{
		canvas.FillTriangle(leftTipV2F,          leftTipInnerV2F,     rightTipV2F); // Tip
	}
	if(drawMiddle)
	{
		canvas.FillTriangle(leftTipV2F,          leftOuterV2F,        leftTipInnerV2F); // Left Outer
		canvas.FillTriangle(leftOuterV2F,        leftInnerV2F,        leftTipInnerV2F); // Left Inner
		canvas.FillTriangle(rightTipV2F,         rightOuterV2F,       rightTipInnerV2F); // Right Outer
		canvas.FillTriangle(rightOuterV2F,       rightInnerV2F,       rightTipInnerV2F); // Right Inner
	}
	if(drawBottom)
	{
		canvas.FillTriangle(leftOuterV2F,        leftBottomOuterV2F,  leftBottomSplitV2F); // Left Bottom outer
		canvas.FillTriangle(leftBottomSplitV2F,  leftBottomOuterV2F,  leftBottomInnerV2F); // Left Bottom split
		canvas.FillTriangle(leftBottomSplitV2F,  leftBottomInnerV2F,  leftInnerV2F); // Left Bottom inner
		canvas.FillTriangle(rightOuterV2F,       rightBottomOuterV2F, rightBottomSplitV2F); // Right Bottom outer
		canvas.FillTriangle(rightBottomSplitV2F, rightBottomOuterV2F, rightBottomInnerV2F); // Right Bottom split
		canvas.FillTriangle(rightBottomSplitV2F, rightBottomInnerV2F, rightInnerV2F); // Right Bottom inner
	}

	/* DRAW LINES */
	if(showLines)
	{
		if(drawTip)
		{
			canvas.DrawLine(leftTipV2F,           rightTipV2F,         2); // Tip
		}
		if(drawMiddle)
		{
			canvas.DrawLine(leftTipV2F,           leftOuterV2F,        2); // Left Tip to outer
			canvas.DrawLine(leftInnerV2F,         leftTipInnerV2F,     2); // Left Inner to tip inner
			canvas.DrawLine(leftTipInnerV2F,      leftTipV2F,          2); // Left Tip inner to tip
			canvas.DrawLine(leftOuterV2F,         leftTipInnerV2F,     2); // Left Outer to tip inner
			canvas.DrawLine(rightTipV2F,          rightOuterV2F,       2); // Right Tip to outer
			canvas.DrawLine(rightInnerV2F,        rightTipInnerV2F,    2); // Right Inner to tip inner
			canvas.DrawLine(rightTipInnerV2F,     rightTipV2F,         2); // Right Tip inner to tip
			canvas.DrawLine(rightOuterV2F,        rightTipInnerV2F,    2); // Right Outer to tip inner
		}
		if(drawBottom)
		{
			canvas.DrawLine(leftOuterV2F,         leftBottomOuterV2F,  2); // Left Outer to bottom outer
			canvas.DrawLine(leftBottomOuterV2F,   leftBottomInnerV2F,  2); // Left Bottom outer to bottom inner
			canvas.DrawLine(leftBottomInnerV2F,   leftInnerV2F,        2); // Left Bottom inner to inner
			canvas.DrawLine(leftBottomOuterV2F,   leftBottomSplitV2F,  2); // Left Bottom outer to bottomsplit
			canvas.DrawLine(leftBottomInnerV2F,   leftBottomSplitV2F,  2); // Left Bottom inner to bottomsplit
			canvas.DrawLine(rightOuterV2F,        rightBottomOuterV2F, 2); // Right Outer to bottom outer
			canvas.DrawLine(rightBottomOuterV2F,  rightBottomInnerV2F, 2); // Right Bottom outer to bottom inner
			canvas.DrawLine(rightBottomInnerV2F,  rightInnerV2F,       2); // Right Bottom inner to inner
			canvas.DrawLine(rightBottomOuterV2F,  rightBottomSplitV2F, 2); // Right Bottom outer to bottomsplit
			canvas.DrawLine(rightBottomInnerV2F,  rightBottomSplitV2F, 2); // Right Bottom inner to bottomsplit
		}
		
		canvas.DrawLine(leftOuterV2F,         leftInnerV2F,        2); // Left Outer to inner
		canvas.DrawLine(rightOuterV2F,        rightInnerV2F,       2); // Right Outer to inner
	}
}

void RT::Chevron::DrawAlongLine(CanvasWrapper canvas, Frustum &frustum, Vector start, Vector end, float gap, float speed, float secondsElapsed) const
{
	//Speed should be given in cm/s

	Vector lineDir = end - start; lineDir.normalize();
	Vector fullEnd = end + lineDir * GetFullLength();

	if((fullEnd - start).magnitude() < 0.001)
	{
		return;
	}

	float lineLengthDiv = 1 / (fullEnd - start).magnitude();

	float animationPercPerSecond = speed * lineLengthDiv;
	float animationPercentage = animationPercPerSecond * secondsElapsed;

	float overflowPerc = GetFullLength() * lineLengthDiv;
	float gapPerc = (gap + GetTipLength()) * lineLengthDiv;
	
	//remove the whole integer portion of the percentage
	animationPercentage -= static_cast<int>(animationPercentage);

	//Get number of chevrons to draw
	int numChevys = static_cast<int>(1.f / gapPerc);
	if(numChevys == 0)
	{
		numChevys = 1;
	}

	//Re-space the chevrons evenly
	gapPerc = 1.f / numChevys;

	for(int i = 0; i < numChevys; ++i)
	{
		float localAnimPerc = animationPercentage + (gapPerc * i);
		localAnimPerc -= static_cast<int>(localAnimPerc);

		Chevron chevy = *this;
		chevy.location = (start * (1 - localAnimPerc) + fullEnd * localAnimPerc);

		if(1 - localAnimPerc < overflowPerc)
		{
			chevy.SetWipeTipToTail(1 - ((1 - localAnimPerc) / overflowPerc));
		}

		if(localAnimPerc < overflowPerc)
		{
			chevy.SetWipeTailToTip(1 - (localAnimPerc / overflowPerc));
		}

		chevy.Draw(canvas, frustum);
	}
}

float RT::Chevron::GetFullLength() const
{
	Vector bottomOuterVert = Vector{length, width, 0};
	Vector tipToOuterDir = bottomOuterVert; tipToOuterDir.normalize();
	Vector outerRightAxis = Vector::cross(tipToOuterDir, Vector{0, 0, 1}); outerRightAxis.normalize();
	Vector bottomInnerVert = bottomOuterVert + (outerRightAxis * thickness);

	return bottomInnerVert.X;
}

float RT::Chevron::GetTipLength() const
{
	return (tipInnerVert - tipVert).magnitude();
}

void RT::Chevron::SetLength(const float newLength)
{
	length = newLength;
	UpdateMaxThickness();
	UpdateBaseVertices();
}

void RT::Chevron::SetWidth(const float newWidth)
{
	width = newWidth;
	UpdateMaxThickness();
	UpdateBaseVertices();
}

void RT::Chevron::SetThickness(const float newThickness)
{
	if(newThickness > maxThickness)
	{
		thickness = maxThickness;
	}
	else
	{
		thickness = newThickness;
	}
	UpdateBaseVertices();
}

void RT::Chevron::UpdateMaxThickness()
{
	Vector tipVert = Vector(0,0,0);
	Vector bottomOuterVert = tipVert + Vector{length, width, 0};
	
	Vector tipToOuterDir = bottomOuterVert - tipVert; tipToOuterDir.normalize();
	Vector outerRightAxis = Vector::cross(tipToOuterDir, Vector{0, 0, 1}); outerRightAxis.normalize();
	Vector bottomInnerVert = bottomOuterVert + (outerRightAxis * thickness);//dot product remains the same regardless of thickness, so we can use thickness here

	float dotOuterVert = Vector::dot(Vector(0, -1, 0) - bottomOuterVert, outerRightAxis);
	float angleInnerSplitVert = (CONST_PI_F / 2.f) - acosf(dotOuterVert);//right angle minus the known angle

	maxThickness = length * cosf(angleInnerSplitVert);
}

void RT::Chevron::SetWipeTipToTail(const float newWipeTipToTail)
{
	wipeTipToTail = newWipeTipToTail;
	if(wipeTipToTail > (1 - wipeTailToTip))
	{
		wipeTailToTip = 1 - wipeTipToTail;
	}
	UpdateBaseVertices();
}

void RT::Chevron::SetWipeTailToTip(const float newWipeTailToTip)
{
	wipeTailToTip = newWipeTailToTip;
	if(wipeTailToTip > (1 - wipeTipToTail))
	{
		wipeTipToTail = 1 - wipeTailToTip;
	}
	UpdateBaseVertices();
}

float RT::Chevron::GetLength() const { return length; }

float RT::Chevron::GetWidth() const { return width; }

float RT::Chevron::GetThickness() const { return thickness; }

float RT::Chevron::GetMaxThickness() const { return maxThickness; }

float RT::Chevron::GetWipeTipToTail() const { return wipeTipToTail; }

float RT::Chevron::GetWipeTailToTip() const { return wipeTailToTip; }

void RT::Chevron::UpdateBaseVertices()
{
	//Recalculate positions of vertices when parameters are changed, or when chevron is constructed

	//Create outer line to find outer vertex
	tipVert = Vector(0,0,0);
	outerVert = tipVert + Vector(length, width, 0);

	//Create local right axis to find thickness and get bottom vertex
	Vector tipToOuterDir = outerVert - tipVert; tipToOuterDir.normalize();
	Vector outerRightAxis = Vector::cross(tipToOuterDir, Vector(0,0,1));
	bottomVert = outerVert + (outerRightAxis * thickness);

	//Find inner line to get inner tip vertex
	Vector tipPlusLocalRight = tipVert + (outerRightAxis * thickness);
	Plane intersectionPlane(Vector(0, 1, 0));
	Line innerLine(bottomVert, tipPlusLocalRight);//fix this
	tipInnerVert = intersectionPlane.LinePlaneIntersectionPoint(innerLine);

	//Find horizontal point on inner line
	Vector innerMidline = (bottomVert + tipInnerVert) * 0.5 + Vector(0,0,50);
	//Plane innerMidlineIntersectionPlane = Triangle(bottomVert, tipInnerVert, innerMidline).GetPlaneFromTriangle();
	Plane innerMidlineIntersectionPlane(outerRightAxis, tipInnerVert);
	Line bottomHorizontalLine(outerVert, outerVert - Vector(0,1,0) * width * 2);//fix this
	innerVert = innerMidlineIntersectionPlane.LinePlaneIntersectionPoint(bottomHorizontalLine);

	//Find lower split vertex
	bottomSplitVert = (outerVert + innerVert) * 0.5f;

	UpdateSegmentPercentages();
}

void RT::Chevron::UpdateSegmentPercentages()
{
	float full = GetFullLength();
	topPerc = (tipInnerVert.X - tipVert.X) / full;
	bottomPerc = (bottomVert.X - bottomSplitVert.X) / full;
	middlePerc = 1.f - topPerc - bottomPerc;
}
