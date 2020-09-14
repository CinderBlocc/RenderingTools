#pragma once

//BakkesMod Struct Includes
#include "bakkesmod/wrappers/wrapperstructs.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"

//3D Objects
#include "Objects/Chevron.h"
#include "Objects/Circle.h"
#include "Objects/Circle2D.h"
#include "Objects/Cone.h"
#include "Objects/Cube.h"
#include "Objects/Cylinder.h"
#include "Objects/Frustum.h"
#include "Objects/Grid.h"
#include "Objects/Line.h"
#include "Objects/Matrix3.h"
#include "Objects/Plane.h"
#include "Objects/Plane.h"
#include "Objects/Sphere.h"
#include "Objects/Triangle.h"
//#include "Objects/VisualCamera.h" //Will reinclude when it's been cleaned up

//Extra Tools
#include "Extra/RenderingAssistant.h"
#include "Extra/RenderingMath.h"
#include "Extra/WrapperStructsExtensions.h"


//All of these will be split off into the appropriate headers eventually
namespace RT
{
	//Rotations
	Matrix3 SingleAxisAlignment(Matrix3 matrix, Vector targetDirection, LookAtAxis axis, int step);
	Matrix3 GetRotationOrder(Matrix3 inMatrix, LookAtAxis axis, int step);
	bool ShouldNegateAngle(float dot, int step);	

	//Miscellaneous canvas functions
	LinearColor GetPercentageColor(float percent, float alpha=1.0f);
	float GetVisualDistance(CanvasWrapper canvas, Frustum frustum, CameraWrapper camera, Vector objectLocation);
	//void SetColor(CanvasWrapper canvas, LinearColor color); //Remove when the corresponding CanvasWrapper function has been added
	void SetColor(CanvasWrapper canvas, std::string colorName, float opacity=255); //Remove as well?
	void DrawDebugStrings(CanvasWrapper canvas, std::vector<std::string> drawString, bool background=false, int width=200);
}