#pragma once
#include "bakkesmod/wrappers/wrapperstructs.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "Frustum.h"

namespace RT
{
	class Chevron
	{
	public:
		Vector location;
		Quat orientation;

		// CONSTRUCTORS
		explicit Chevron();
		explicit Chevron(Vector loc, Quat rot);
		explicit Chevron(Vector loc, Quat rot, float len, float wid, float thicc, float tipToTail, float tailToTip);

		// FUNCTIONS
		void Draw(CanvasWrapper canvas, Frustum frustum, bool showLines=false);
		void DrawAlongLine(CanvasWrapper canvas, Frustum frustum, Vector start, Vector end, float gap, float speed, float secondsElapsed);

		float GetLength();
		void SetLength(const float newLength);
		float GetFullLength();
		float GetTipLength();
		float GetWidth();
		void SetWidth(const float newWidth);
		float GetThickness();
		void SetThickness(const float newThickness);
		float GetMaxThickness();
		float GetWipeTipToTail();
		void SetWipeTipToTail(const float newWipeTipToTail);
		float GetWipeTailToTip();
		void SetWipeTailToTip(const float newWipeTailToTip);

	private:
		//Due to the nature of how the chevron is drawn
		//these values must be private and accessed via
		//their respective Get/Set functions
		float length;
		float width;
		float thickness;
		float maxThickness;
		float wipeTipToTail;
		float wipeTailToTip;
		void UpdateMaxThickness();
		void UpdateBaseVertices();

		//These values below are only useful for the drawing functions
		float topPerc = .33f;
		float middlePerc = .33f;
		float bottomPerc = .33f;
		void UpdateSegmentPercentages();
		Vector tipVert;
		Vector outerVert;
		Vector bottomVert;
		Vector innerVert;
		Vector tipInnerVert;
		Vector bottomSplitVert;
	};
}