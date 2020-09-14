#include "bakkesmod/wrappers/canvaswrapper.h"
#include "Line.h"
#include "Frustum.h"
#include <vector>

RT::Line::Line()
	: lineBegin(Vector{0,0,0}), lineEnd(Vector{0,0,0}), thickness(1.f) {}

RT::Line::Line(Vector begin, Vector end)
	: lineBegin(begin), lineEnd(end), thickness(1) {}

RT::Line::Line(Vector begin, Vector end, float thicc)
	: lineBegin(begin), lineEnd(end), thickness(thicc) {}

void RT::Line::Draw(CanvasWrapper canvas) const
{
	if(thickness == 1)
	{
		canvas.DrawLine(canvas.ProjectF(lineBegin), canvas.ProjectF(lineEnd));
	}
	else
	{
		canvas.DrawLine(canvas.ProjectF(lineBegin), canvas.ProjectF(lineEnd), thickness);
	}
}

void RT::Line::DrawWithinFrustum(CanvasWrapper canvas, Frustum &frustum) const
{
    Line thisLine = *this;

	//Check if the beginning and the end of the line are in the frustum
	bool beginInFrustum = frustum.IsInFrustum(lineBegin);
	bool endInFrustum = frustum.IsInFrustum(lineEnd);
	
	//If line is completely inside frustum, draw normally and return
	if(beginInFrustum && endInFrustum)
	{
		Draw(canvas);
		return;
	}
	
	//Store which planes have been intersected and the location
	std::vector<int> planeIndex;
	for(int i = 0; i != 6; ++i)
	{
		if(frustum.planes[i].LineIntersectsWithPlane(thisLine))
		{
			Vector intersectLocation = frustum.planes[i].LinePlaneIntersectionPoint(thisLine);
			if(frustum.IsInFrustum(intersectLocation, 1.f) && IsPointWithinLineSegment(intersectLocation))
			{
				planeIndex.push_back(i);
			}
		}
	}

	//If no planes have been intersected, the line does not pass through the frustum. Don't draw
	if(planeIndex.empty())
	{
		return;
	}

	/* UPDATE LINE */
	Line tempLine = *this;

	//Update the beginning if it's the only beginning outside the frustum
	if(!beginInFrustum && endInFrustum)
	{
		tempLine.lineBegin = frustum.planes[planeIndex[0]].LinePlaneIntersectionPoint(thisLine);
		tempLine.Draw(canvas);
		return;
	}

	//Update the end if it's the only end outside the frustum
	if(beginInFrustum && !endInFrustum)
	{
		tempLine.lineEnd = frustum.planes[planeIndex[0]].LinePlaneIntersectionPoint(thisLine);
		tempLine.Draw(canvas);
		return;
	}

	//Update both the beginning and the end because they're both outside the frustum
	//Get both locations, then move each line point to the nearest location
	if(planeIndex.size() < 2)
	{
		return;
	}
	Vector intersect0 = frustum.planes[planeIndex[0]].LinePlaneIntersectionPoint(thisLine);
	Vector intersect1 = frustum.planes[planeIndex[1]].LinePlaneIntersectionPoint(thisLine);

	tempLine.lineBegin = ((tempLine.lineBegin - intersect0).magnitude() < (tempLine.lineBegin - intersect1).magnitude())
		? intersect0 : intersect1;

	tempLine.lineEnd = ((tempLine.lineEnd - intersect0).magnitude() < (tempLine.lineEnd - intersect1).magnitude())
		? intersect0 : intersect1;

	tempLine.Draw(canvas);
}

void RT::Line::DrawSegmentedManual(CanvasWrapper canvas, Frustum &frustum, float animationPerc, int segments, float segPercent) const
{
	//MANUAL: Manually define number of desired segments and the percentage of each segment that should be visible

	if(segments == 0) return; //avoid divide by 0 errors

	//Subtract the whole value amount from animationPerc to get just the 0-1 value
	float trueAnimPerc = abs(animationPerc) - static_cast<int>(abs(animationPerc));

	//Generate segments
	std::vector<float> splits;
	for(int i = 0; i != segments; ++i)
	{
		float position = (static_cast<float>(i) / segments) + trueAnimPerc;
		splits.push_back(position);
	}

	//Draw segments
	float lineLength = magnitude();
	for(size_t i = 0; i != splits.size(); ++i)
	{
		//Reset split to start of line if it has overflowed
		if(splits[i] > 1)
		{
			splits[i] -= 1;
		}

		//Get the correct beginning and end values
		float startPerc = splits[i];
		float endPerc = splits[i] + (segPercent / segments);

		//If segment end extends past end point, take remainder and draw it at the front. Cap the real end value to the line end
		if(endPerc > 1)
		{
			Vector overflowStart = lineBegin;
			Vector overflowEnd = lineBegin + (direction() * lineLength * (endPerc - 1));
			if(frustum.IsInFrustum(overflowStart, 20) && frustum.IsInFrustum(overflowEnd, 20))
			{
				if(thickness != 1)
					canvas.DrawLine(canvas.ProjectF(overflowStart), canvas.ProjectF(overflowEnd), thickness);
				else
					canvas.DrawLine(canvas.ProjectF(overflowStart), canvas.ProjectF(overflowEnd));
			}

			//Set normal end to cap at 1
			endPerc = 1;
		}

		//Draw line segment
		Vector start = lineBegin + (direction() * lineLength * startPerc);
		Vector end = lineBegin + (direction() * lineLength * endPerc);
		if(frustum.IsInFrustum(start, 20) && frustum.IsInFrustum(end, 20))
		{
			if(thickness != 1)
				canvas.DrawLine(canvas.ProjectF(start), canvas.ProjectF(end), thickness);
			else
				canvas.DrawLine(canvas.ProjectF(start), canvas.ProjectF(end));
		}
	}
}

void RT::Line::DrawSegmentedAutomatic(CanvasWrapper canvas, Frustum &frustum, float segmentLength, float gapLength, float speed, float secondsElapsed)
{
	//AUTOMATIC: Calculates number of segments and segPercent automatically based on desired segment and gap length
	//Speed should be given in cm/s

	if(segmentLength > magnitude())
	{
		segmentLength = magnitude();
		gapLength = 0;
	}
	float totalSegmentLength = segmentLength + gapLength;
	
	//Avoid divide by 0 error
	if(totalSegmentLength == 0) return;
	
	//Calculate number of segments and length of each segment
	int segs = static_cast<int>(magnitude() / totalSegmentLength);
	float segPerc = segmentLength / totalSegmentLength;

	//Convert distance units into line percentage
	float animationPercPerSecond = (speed * 100) / magnitude();// distance along line percentage
	animationPercentage += animationPercPerSecond * secondsElapsed;
	
	//if(animationPercentage >= 1)
	//{
		//remove the whole integer portion of the percentage
		animationPercentage -= static_cast<int>(animationPercentage);
	//}

	DrawSegmentedManual(canvas, frustum, animationPercentage, segs, segPerc);
}

bool RT::Line::IsPointWithinLineSegment(Vector point) const
{
	Vector beginToPoint = lineBegin - point;
	beginToPoint.normalize();

	//Check if point is colinear with line
	float cross = Vector::cross(direction(), beginToPoint).magnitude();
	if(cross > 0.001)
	{
		return false;
	}

	//Check if point is between the beginning and end of the line
	float perc = PointPercentageAlongLine(point);
	if(perc < 0 || perc > 1)
	{
		return false;
	}
	
	return true;
}

float RT::Line::PointPercentageAlongLine(Vector point) const
{
	float dot = Vector::dot((lineEnd - lineBegin),(point - lineBegin));
	return (dot / (magnitude() * magnitude()));
}

Vector RT::Line::GetPointAlongLine(float percent) const
{
	return (lineBegin * (1 - percent) + lineEnd * percent);
}

Vector RT::Line::direction() const
{
	Vector dir = lineEnd - lineBegin;
	dir.normalize();
	return dir;
}

const float RT::Line::magnitude() const
{
	return (lineEnd - lineBegin).magnitude();
}

const float RT::Line::getAnimationPercentage() const
{
	return animationPercentage;
}

void RT::Line::setAnimationPercentage(float perc)
{
	animationPercentage = perc;
}
