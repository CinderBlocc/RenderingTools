#pragma once
#include "bakkesmod/wrappers/wrapperstructs.h"

class CanvasWrapper;

namespace RT
{
    class Frustum;

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
		void Draw(CanvasWrapper canvas, Frustum &frustum, bool showLines=false) const;
		void DrawAlongLine(CanvasWrapper canvas, Frustum &frustum, Vector start, Vector end, float gap, float speed, float secondsElapsed) const;

		float GetLength() const;
		void SetLength(const float newLength);
		float GetFullLength() const;
		float GetTipLength() const;
		float GetWidth() const;
		void SetWidth(const float newWidth);
		float GetThickness() const;
		void SetThickness(const float newThickness);
		float GetMaxThickness() const;
		float GetWipeTipToTail() const;
		void SetWipeTipToTail(const float newWipeTipToTail);
		float GetWipeTailToTip() const;
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
		float topPerc = 0.33f;
		float middlePerc = 0.33f;
		float bottomPerc = 0.33f;
		void UpdateSegmentPercentages();
		Vector tipVert;
		Vector outerVert;
		Vector bottomVert;
		Vector innerVert;
		Vector tipInnerVert;
		Vector bottomSplitVert;
	};
}