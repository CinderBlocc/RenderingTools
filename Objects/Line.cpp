#include "Line.h"
#include "Frustum.h"

void RT::Line::Draw(CanvasWrapper canvas)
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

/*
//ORIGINAL - Dont touch until new one works
void RT::Line::DrawWithinFrustum(CanvasWrapper canvas, Frustum frustum, Vector &newLineBegin, Vector &newLineEnd)
{
	//RELIES ON OLDER BUGGY VERSION OF IsWithinLineSegment
	//UPDATE THIS TO CHANGE THE BEGIN AND END POINTS OF A COPY OF THE LINE TO FIT INSIDE THE FRUSTUM

	//return 0 if line is completely inside frustum
	//return 1 if line is completely outside frustum
	//return 2 if lineBegin is outside frustum
	//return 3 if lineEnd is outside frustum
	//return 4 if both are out of frustum but line still intersects

	int pointsOutsideFrustum = 0;
	int numIntersections = 0;
	bool beginInFrustum = false;
	bool endInFrustum = false;
	bool intersectsInFrustum[2] = {false, false};
	Vector intersects[2];

	if(IsInFrustum(frustum, line.lineBegin, 0.0f))
		beginInFrustum = true;
	else
		pointsOutsideFrustum++;
	if(IsInFrustum(frustum, line.lineEnd, 0.0f))
		endInFrustum = true;
	else
		pointsOutsideFrustum++;
	
	if(beginInFrustum && endInFrustum)
		return 0;//Line is completely inside frustum
	else
	{
		int intersectionIndex = 0;
		for(int i=0; i<6; i++)
		{
			if(LinePlaneIntersection(line, frustum.planes[i]))
			{
				Vector intersection = LinePlaneIntersectionPoint(line, frustum.planes[i]);
				if(IsInFrustum(frustum, intersection, 1.0f))
				{
					intersectsInFrustum[intersectionIndex] = true;
					intersects[intersectionIndex] = intersection;
					intersectionIndex++;
				}
			}
		}

		if(!intersectsInFrustum[0] && !intersectsInFrustum[1])
			return 1;//Line is completely outside frustum
		if(!beginInFrustum && endInFrustum)
		{
			if(IsWithinLineSegment(line, intersects[0], false))
				newLineBegin = Vector{intersects[0].X, intersects[0].Y, intersects[0].Z};
			else
				newLineBegin = Vector{intersects[1].X, intersects[1].Y, intersects[1].Z};
			return 2;//Only lineBegin is outside frustum and should be replaced by newLineBegin
		}
		if(beginInFrustum && !endInFrustum)
		{
			if(IsWithinLineSegment(line, intersects[0], true))
				newLineEnd = Vector{intersects[0].X, intersects[0].Y, intersects[0].Z};
			else
				newLineEnd = Vector{intersects[1].X, intersects[1].Y, intersects[1].Z};
			return 3;//Only lineEnd is outside frustum and should be replaced by newLineEnd
		}
		if(intersectsInFrustum[0] && intersectsInFrustum[1])
		{
			newLineBegin = Vector{intersects[0].X, intersects[0].Y, intersects[0].Z};
			newLineEnd = Vector{intersects[1].X, intersects[1].Y, intersects[1].Z};
			return 4;//Both lineBegin and lineEnd are both outside frustum and should be replaced by newLineBegin and newLineEnd
		}
	}
}
*/

void RT::Line::DrawWithinFrustum(CanvasWrapper canvas, Frustum frustum)
{
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
	vector<int> planeIndex;
	for(int i = 0; i != 6; ++i)
	{
		if(frustum.planes[i].LineIntersectsWithPlane(*this))
		{
			Vector intersectLocation = frustum.planes[i].LinePlaneIntersectionPoint(*this);
			if(frustum.IsInFrustum(intersectLocation, 1.f) && IsPointWithinLineSegment(intersectLocation))
			{
				planeIndex.push_back(i);
			}
		}
	}

	//If no planes have been intersected, the line does not pass through the frustum. Don't draw
	if(planeIndex.size() == 0)
	{
		return;
	}

	/* UPDATE LINE */
	Line tempLine = *this;

	//Update the beginning if it's the only beginning outside the frustum
	if(!beginInFrustum && endInFrustum)
	{
		tempLine.lineBegin = frustum.planes[planeIndex[0]].LinePlaneIntersectionPoint(*this);
		tempLine.Draw(canvas);
		return;
	}

	//Update the end if it's the only end outside the frustum
	if(beginInFrustum && !endInFrustum)
	{
		tempLine.lineEnd = frustum.planes[planeIndex[0]].LinePlaneIntersectionPoint(*this);
		tempLine.Draw(canvas);
		return;
	}

	//Update both the beginning and the end because they're both outside the frustum
	//Get both locations, then move each line point to the nearest location
	if(planeIndex.size() < 2)
	{
		return;
	}
	Vector intersect0 = frustum.planes[planeIndex[0]].LinePlaneIntersectionPoint(*this);
	Vector intersect1 = frustum.planes[planeIndex[1]].LinePlaneIntersectionPoint(*this);

	tempLine.lineBegin = ((tempLine.lineBegin - intersect0).magnitude() < (tempLine.lineBegin - intersect1).magnitude())
		? intersect0 : intersect1;

	tempLine.lineEnd = ((tempLine.lineEnd - intersect0).magnitude() < (tempLine.lineEnd - intersect1).magnitude())
		? intersect0 : intersect1;

	tempLine.Draw(canvas);
	return;


	//return 0 if line is completely inside frustum
	//return 1 if line is completely outside frustum
	//return 2 if lineBegin is outside frustum
	//return 3 if lineEnd is outside frustum
	//return 4 if both are out of frustum but line still intersects

	/*
	int intersectionIndex = 0;
	for(int i=0; i<6; i++)
	{
		if(frustum.planes[i].LineIntersectsWithPlane(*this))
		{
			Vector intersection = frustum.planes[i].LinePlaneIntersectionPoint(*this);
			if(frustum.IsInFrustum(intersection))
			{
				intersectsInFrustum[intersectionIndex] = true;
				intersects[intersectionIndex] = intersection;
				intersectionIndex++;
			}
		}
	}

	if(!intersectsInFrustum[0] && !intersectsInFrustum[1])
		return 1;//Line is completely outside frustum
	if(!beginInFrustum && endInFrustum)
	{
		if(IsWithinLineSegment(line, intersects[0], false))
			newLineBegin = Vector{intersects[0].X, intersects[0].Y, intersects[0].Z};
		else
			newLineBegin = Vector{intersects[1].X, intersects[1].Y, intersects[1].Z};
		return 2;//Only lineBegin is outside frustum and should be replaced by newLineBegin
	}
	if(beginInFrustum && !endInFrustum)
	{
		if(IsWithinLineSegment(line, intersects[0], true))
			newLineEnd = Vector{intersects[0].X, intersects[0].Y, intersects[0].Z};
		else
			newLineEnd = Vector{intersects[1].X, intersects[1].Y, intersects[1].Z};
		return 3;//Only lineEnd is outside frustum and should be replaced by newLineEnd
	}
	if(intersectsInFrustum[0] && intersectsInFrustum[1])
	{
		newLineBegin = Vector{intersects[0].X, intersects[0].Y, intersects[0].Z};
		newLineEnd = Vector{intersects[1].X, intersects[1].Y, intersects[1].Z};
		return 4;//Both lineBegin and lineEnd are both outside frustum and should be replaced by newLineBegin and newLineEnd
	}
	*/
}

void RT::Line::DrawSegmentedManual(CanvasWrapper canvas, Frustum &frustum, float animationPerc, int segments, float segPercent)
{
	//MANUAL: Manually define number of desired segments and the percentage of each segment that should be visible

	if(segments == 0) return; //avoid divide by 0 errors

	//Subtract the whole value amount from animationPerc to get just the 0-1 value
	float trueAnimPerc = abs(animationPerc) - static_cast<int>(abs(animationPerc));

	//Generate segments
	vector<float> splits;
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

void RT::Line::DrawSegmentedAutomatic(CanvasWrapper canvas, Frustum &frustum, float segmentLength, float gapLength, float speed, double secondsElapsed)
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
	int segs = magnitude() / totalSegmentLength;
	float segPerc = segmentLength / totalSegmentLength;

	//Convert distance units into line percentage
	float animationPercPerSecond = (speed * 100) / magnitude();// distance along line percentage
	animationPercentage += animationPercPerSecond * secondsElapsed;
	
	//if(animationPercentage >= 1)
	//{
		//remove the whole integer portion of the percentage
		animationPercentage -= (int)animationPercentage;
	//}

	DrawSegmentedManual(canvas, frustum, animationPercentage, segs, segPerc);
}

bool RT::Line::IsPointWithinLineSegment(Vector point)
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

float RT::Line::PointPercentageAlongLine(Vector point)
{
	float dot = Vector::dot((lineEnd - lineBegin),(point - lineBegin));
	return (dot / (magnitude() * magnitude()));
}

Vector RT::Line::GetPointAlongLine(float percent)
{
	return (lineBegin * (1 - percent) + lineEnd * percent);
}

Vector RT::Line::direction()
{
	Vector direction = lineEnd - lineBegin;
	direction.normalize();
	return direction;
}

const float RT::Line::magnitude()
{
	return (lineEnd - lineBegin).magnitude();
}

const float RT::Line::getAnimationPercentage()
{
	return animationPercentage;
}

void RT::Line::setAnimationPercentage(float perc)
{
	animationPercentage = perc;
}
