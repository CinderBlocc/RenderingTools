#pragma once
#include "bakkesmod/wrappers/wrapperstructs.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"

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
		void Draw(CanvasWrapper canvas);
		void DrawWithinFrustum(CanvasWrapper canvas, Frustum frustum);
		void DrawSegmentedManual(CanvasWrapper canvas, Frustum &frustum, float animationPerc, int segments=10, float segPercent=0.5f);
		void DrawSegmentedAutomatic(CanvasWrapper canvas, Frustum &frustum, float segmentLength, float gapLength, float speed=0, double secondsElapsed=0);

		bool IsPointWithinLineSegment(Vector point);
		float PointPercentageAlongLine(Vector point);
		Vector GetPointAlongLine(float percent);

		Vector direction();
		const float magnitude();
		const float getAnimationPercentage();
		void setAnimationPercentage(float perc);

	private:
		float animationPercentage = 0.f;
	};
}
