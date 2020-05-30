#pragma once

//BakkesMod Struct Includes
#include "bakkesmod/wrappers/wrapperstructs.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"

//Rendering Tools Includes
#include "Objects/Chevron.h"
#include "Objects/Circle.h"
#include "Objects/Cone.h"
#include "Objects/Cube.h"
#include "Objects/Frustum.h"
#include "Objects/Grid.h"
#include "Objects/Line.h"
#include "Objects/Matrix3.h"
#include "Objects/Plane.h"
#include "Objects/Plane.h"
#include "Objects/Sphere.h"
#include "Objects/Triangle.h"
//#include "Objects/VisualCamera.h" //Will reinclude when it's been cleaned up
#include "Extra/RenderingAssistant.h"
#include "Extra/RenderingMath.h"
#include "Extra/WrapperStructsExtensions.h"

namespace RT
{
	//Rotations
	Matrix3 SingleAxisAlignment(Matrix3 matrix, Vector targetDirection, LookAtAxis axis, int step);
	Matrix3 GetRotationOrder(Matrix3 inMatrix, LookAtAxis axis, int step);
	bool ShouldNegateAngle(float dot, int step);	

	//Miscellaneous canvas functions
	LinearColor GetPercentageColor(float percent, float alpha=1.0f);
	float GetVisualDistance(CanvasWrapper canvas, Frustum frustum, CameraWrapper camera, Vector objectLocation);
	void SetColor(CanvasWrapper canvas, LinearColor color);
	void SetColor(CanvasWrapper canvas, string colorName, float opacity=255);
	void DrawDebugStrings(CanvasWrapper canvas, vector<string> drawString, bool background=false, int width=200);
}