#pragma once
#include "bakkesmod/wrappers/wrapperstructs.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "Plane.h"

namespace RT
{
	class Frustum
	{
	public:
		Vector points[8]; // FTL, FTR, FBR, FBL, NTL, NTR, NBR, NBL
		Plane planes[6]; // Top, Bottom, Left, Right, Near, Far

		// CONSTRUCTORS
		explicit Frustum() {}
		explicit Frustum(CanvasWrapper canvas, Quat cameraQuat, Vector cameraLocation, float FOV=90, float nearClip=50, float farClip=20000); // Useful for drawing a frustum (used in VisualCamera)
		explicit Frustum(CanvasWrapper canvas, CameraWrapper camera, float nearClip=50, float farClip=20000); // Useful for culling. Frustum(canvas, camera);
		
		// FUNCTIONS
		void Draw(CanvasWrapper canvas);

		bool IsInFrustum(Vector position, float radius=0.f);
	};
}
