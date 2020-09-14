#include "bakkesmod/wrappers/canvaswrapper.h"
#include "bakkesmod/wrappers/GameObject/CameraWrapper.h"
#include "Frustum.h"
#include "Matrix3.h"
#include "Triangle.h"
#include "Plane.h"
#include "../Extra/WrapperStructsExtensions.h"

RT::Frustum::Frustum(CanvasWrapper canvas, Quat cameraQuat, Vector cameraLocation, float FOV, float nearClip, float farClip)
{
	//Generate the 6 planes of the viewing frustum from the 8 vertices of the frustum
	//https://www.lighthouse3d.com/tutorials/view-frustum-culling/geometric-approach-extracting-the-planes/

	Quat fQuat = cameraQuat * Quat(0,1,0,0) * cameraQuat.conjugate();
	Quat rQuat = cameraQuat * Quat(0,0,1,0) * cameraQuat.conjugate();
	Quat uQuat = cameraQuat * Quat(0,0,0,1) * cameraQuat.conjugate();

	Matrix3 mat(
		Vector{fQuat.X, fQuat.Y, fQuat.Z},
		Vector{rQuat.X, rQuat.Y, rQuat.Z},
		Vector{uQuat.X, uQuat.Y, uQuat.Z}
	);
	mat.normalize();

	float aspectRatio = static_cast<float>(canvas.GetSize().X) / canvas.GetSize().Y;
	float angle = 2.f * tanf(FOV * .5f * (CONST_PI_F / 180));

	float fFarWidth   = angle * farClip;
	float fFarHeight  = fFarWidth / aspectRatio;
	float fNearWidth  = angle * nearClip;
	float fNearHeight = fNearWidth / aspectRatio;

	Vector vFarPlane   = cameraLocation + mat.forward * farClip;
	Vector vNearPlane  = cameraLocation + mat.forward * nearClip;

	Vector vFarHeight  = mat.up    * fFarHeight  * 0.5f;
	Vector vFarWidth   = mat.right * fFarWidth   * 0.5f;
	Vector vNearHeight = mat.up    * fNearHeight * 0.5f;
	Vector vNearWidth  = mat.right * fNearWidth  * 0.5f;

	constexpr int FTL = 0; // Far Top Left
	constexpr int FTR = 1; // Far Top Right
	constexpr int FBR = 2; // Far Bottom Right
	constexpr int FBL = 3; // Far Bottom Left
	constexpr int NTL = 4; // Near Top Left
	constexpr int NTR = 5; // Near Top Right
	constexpr int NBR = 6; // Near Bottom Right
	constexpr int NBL = 7; // Near Bottom Left

	points[FTL] = vFarPlane  + vFarHeight  - vFarWidth;
	points[FTR] = vFarPlane  + vFarHeight  + vFarWidth;
	points[FBR] = vFarPlane  - vFarHeight  + vFarWidth;
	points[FBL] = vFarPlane  - vFarHeight  - vFarWidth;
	points[NTL] = vNearPlane + vNearHeight - vNearWidth;
	points[NTR] = vNearPlane + vNearHeight + vNearWidth;
	points[NBR] = vNearPlane - vNearHeight + vNearWidth;
	points[NBL] = vNearPlane - vNearHeight - vNearWidth;
	
	planes[0] = Triangle{points[FTL], points[FTR], points[NTL]}.GetPlaneFromTriangle(); // Top
	planes[1] = Triangle{points[FBR], points[FBL], points[NBR]}.GetPlaneFromTriangle(); // Bottom
	planes[2] = Triangle{points[FTL], points[NTL], points[FBL]}.GetPlaneFromTriangle(); // Left
	planes[3] = Triangle{points[NTR], points[FTR], points[NBR]}.GetPlaneFromTriangle(); // Right
	planes[4] = Triangle{points[NTL], points[NTR], points[NBL]}.GetPlaneFromTriangle(); // Near
	planes[5] = Triangle{points[FTR], points[FTL], points[FBR]}.GetPlaneFromTriangle(); // Far
}

RT::Frustum::Frustum(CanvasWrapper canvas, CameraWrapper camera, float nearClip, float farClip)
	: Frustum(canvas, RotatorToQuat(camera.GetRotation()), camera.GetLocation(), camera.GetFOV(), nearClip, farClip) {}

void RT::Frustum::Draw(CanvasWrapper canvas) const
{
	constexpr int FTL = 0;
	constexpr int FTR = 1;
	constexpr int FBR = 2;
	constexpr int FBL = 3;
	constexpr int NTL = 4;
	constexpr int NTR = 5;
	constexpr int NBR = 6;
	constexpr int NBL = 7;

	//Far plane
	canvas.DrawLine(canvas.ProjectF(points[FTL]), canvas.ProjectF(points[FTR]), 3);
	canvas.DrawLine(canvas.ProjectF(points[FTR]), canvas.ProjectF(points[FBR]));
	canvas.DrawLine(canvas.ProjectF(points[FBR]), canvas.ProjectF(points[FBL]));
	canvas.DrawLine(canvas.ProjectF(points[FBL]), canvas.ProjectF(points[FTL]));
	//Near plane
	canvas.DrawLine(canvas.ProjectF(points[NTL]), canvas.ProjectF(points[NTR]));
	canvas.DrawLine(canvas.ProjectF(points[NTR]), canvas.ProjectF(points[NBR]));
	canvas.DrawLine(canvas.ProjectF(points[NBR]), canvas.ProjectF(points[NBL]));
	canvas.DrawLine(canvas.ProjectF(points[NBL]), canvas.ProjectF(points[NTL]));
	//Lines between
	canvas.DrawLine(canvas.ProjectF(points[FTL]), canvas.ProjectF(points[NTL]));
	canvas.DrawLine(canvas.ProjectF(points[FTR]), canvas.ProjectF(points[NTR]));
	canvas.DrawLine(canvas.ProjectF(points[FBR]), canvas.ProjectF(points[NBR]));
	canvas.DrawLine(canvas.ProjectF(points[FBL]), canvas.ProjectF(points[NBL]));
}

bool RT::Frustum::IsInFrustum(Vector location, float radius) const
{
	//Check if object is on positive side of all 6 planes of frustum
	for(const auto& plane : planes)
	{
		if(Vector::dot(location, plane.direction()) + plane.d + radius <= 0)
		{
			return false;
		}
	}
	return true;
}
