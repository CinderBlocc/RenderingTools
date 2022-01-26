#pragma once
#include "bakkesmod/wrappers/wrapperstructs.h"

class CanvasWrapper;

namespace RT
{
	class Frustum;

	class Line
	{
	public:
		Vector lineBegin;
		Vector lineEnd;
		float thickness;

		// CONSTRUCTORS
		explicit Line();
		explicit Line(Vector begin, Vector end);
		explicit Line(Vector begin, Vector end, float thicc);
		
		// FUNCTIONS
		void Draw(CanvasWrapper canvas) const;
		void DrawWithinFrustum(CanvasWrapper canvas, Frustum &frustum) const;
		void DrawSegmentedManual(CanvasWrapper canvas, Frustum &frustum, float animationPerc, int32_t segments = 10, float segPercent = 0.5f) const;
		void DrawSegmentedAutomatic(CanvasWrapper canvas, Frustum &frustum, float segmentLength, float gapLength, float speed = 0.0f, float secondsElapsed = 0.0f);

		bool IsPointWithinLineSegment(Vector point) const;
		float PointPercentageAlongLine(Vector point) const;
		Vector GetPointAlongLine(float percent) const;

		Vector direction() const;
		const float magnitude() const;
		const float getAnimationPercentage() const;
		void setAnimationPercentage(float perc);

	private:
		float animationPercentage = 0.f;
	};
}
