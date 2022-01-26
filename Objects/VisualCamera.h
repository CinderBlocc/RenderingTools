#pragma once
#include "bakkesmod/wrappers/wrapperstructs.h"
#include <vector>

class CanvasWrapper;

namespace RT
{
	class VisualCamera
	{
	private:
		Vector originalMatteBoxSide[4];
		Vector originalBodySide[7];
		Vector originalReelSide[12];//These values are offsets from the center of the reel polygon. Need to define center point when creating a new reel.
		Vector originalLens[8];//These are offsets from the center of the lens
		std::vector<Vector> allOriginalCameraVerts;
		std::vector<Vector> allCalculatedCameraVerts;

		void TransformCamera(Vector location, Rotator rotation, float scale);
		void DrawObject(CanvasWrapper canvas, size_t startingIndex, size_t range);

	public:
		explicit VisualCamera();

		void DrawCamera(CanvasWrapper canvas, Vector location, Rotator rotation, float scale = 2.0f, LinearColor color = {255.0f,255.0f,255.0f,255.0f});
		void DrawCameraWithViewFrustum(CanvasWrapper canvas, Vector location, Rotator rotation, float FOV, float clipDistance = 250.0f, float scale = 2.0f, LinearColor color = {255.0f,255.0f,255.0f,255.0f});
	};
}
