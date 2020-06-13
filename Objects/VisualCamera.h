#pragma once
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "../Extra/RenderingMath.h"
#include "Frustum.h"

/*

	NOTE: This will need a lot of overhaul, but is relatively unimportant and will be left for last

*/

namespace RT
{
	class VisualCamera
	{
	private:
		Vector originalMatteBoxSide[4];
		Vector originalBodySide[7];
		Vector originalReelSide[12];//These values are offsets from the center of the reel polygon. Need to define center point when creating a new reel.
		Vector originalLens[8];//These are offsets from the center of the lens
		vector<Vector> allOriginalCameraVerts;
		vector<Vector> allCalculatedCameraVerts;

		void TransformCamera(Vector location, Rotator rotation, float scale);
		void DrawObject(CanvasWrapper canvas, size_t startingIndex, size_t range);

	public:
		explicit VisualCamera();

		void DrawCamera(CanvasWrapper canvas, Vector location, Rotator rotation, float scale=2, LinearColor color={255,255,255,255});
		void DrawCameraWithViewFrustum(CanvasWrapper canvas, Vector location, Rotator rotation, float FOV, float clipDistance=250, float scale=2, LinearColor color={255,255,255,255});
	};
}
