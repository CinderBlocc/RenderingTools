#include "bakkesmod/wrappers/canvaswrapper.h"
#include "VisualCamera.h"
#include "Frustum.h"
#include "../Extra/RenderingMath.h"
#include "../Extra/WrapperStructsExtensions.h"

RT::VisualCamera::VisualCamera()
{
	//Matte box (4 verts)
	originalMatteBoxSide[0] = Vector{ 49.3646f, -19.1627f, 10.9706f };
	originalMatteBoxSide[1] = Vector{ 49.3646f, -19.1627f, -13.8512f};
	originalMatteBoxSide[2] = Vector{ 35.7538f, -7.15999f, -7.56008f};
	originalMatteBoxSide[3] = Vector{ 35.7538f, -7.15999f, 4.67943f };

	//Body (7 verts)
	originalBodySide[0] = Vector{ 21.6909f,  -8.78553f, 5.29488f  };
	originalBodySide[1] = Vector{ 21.6909f,  -8.78552f, -8.17553f };
	originalBodySide[2] = Vector{ 13.3338f,  -8.78552f, -14.3408f };
	originalBodySide[3] = Vector{ -12.42f,   -8.78552f, -14.3408f };
	originalBodySide[4] = Vector{ -19.6006f, -8.78553f, -4.96522f };
	originalBodySide[5] = Vector{ -9.44514f, -8.78553f, 12.487f   };
	originalBodySide[6] = Vector{ 13.2901f,  -8.78553f, 12.487f   };

	//Reel (12 verts)
	originalReelSide[0]  = Vector{ 13.9088f,  0, 3.72685f  };
	originalReelSide[1]  = Vector{ 13.9088f,  0, -3.72686f };
	originalReelSide[2]  = Vector{ 10.1819f,  0, -10.182f  };
	originalReelSide[3]  = Vector{ 3.72684f,  0, -13.9088f };
	originalReelSide[4]  = Vector{ -3.72686f, 0, -13.9088f };
	originalReelSide[5]  = Vector{ -10.182f,  0, -10.1819f };
	originalReelSide[6]  = Vector{ -13.9088f, 0, -3.72685f };
	originalReelSide[7]  = Vector{ -13.9088f, 0, 3.72685f  };
	originalReelSide[8]  = Vector{ -10.1819f, 0, 10.1819f  };
	originalReelSide[9]  = Vector{ -3.72685f, 0, 13.9088f  };
	originalReelSide[10] = Vector{ 3.72685f, 0, 13.9088f   };
	originalReelSide[11] = Vector{ 10.1819f, 0, 10.1819f   };

	//Lens (8 verts)
	originalLens[0] = Vector{ -7.03141f, 0,         5.31923f  };
	originalLens[1] = Vector{ -7.03141f, -3.76126f, 3.76126f  };
	originalLens[2] = Vector{ -7.03141f, -5.31923f, 0         };
	originalLens[3] = Vector{ -7.03141f, -3.76126f, -3.76126f };
	originalLens[4] = Vector{ -7.03141f, 0,         -5.31923f };
	originalLens[5] = Vector{ -7.03141f, 3.76126f,  -3.76126f };
	originalLens[6] = Vector{ -7.03141f, 5.31923f,  0         };
	originalLens[7] = Vector{ -7.03141f, 3.76126f,  3.76126f  };


	//FILL OUT ALL ORIGINAL POINTS
	
	allOriginalCameraVerts.clear();
	//Matte box (8 verts): 0-7
	for(Vector temp : originalMatteBoxSide)
	{
		allOriginalCameraVerts.push_back(temp);
		temp.Y *= -1;
		allOriginalCameraVerts.push_back(temp);
	}
	//Body (14 verts): 8-21
	for(Vector temp : originalBodySide)
	{
		allOriginalCameraVerts.push_back(temp);
		temp.Y *= -1;
		allOriginalCameraVerts.push_back(temp);
	}
	//Reels (24 verts): 22-69
	for(int reel = 0; reel < 2; ++reel)
	{
		Vector reelPosition = {0,0,0};
		if(reel == 0){ reelPosition = Vector{ -1.441f, -8.79f, 26.4f }; };//top reel
		if(reel == 1){ reelPosition = Vector{ -25.531f, -8.79f, 12.49f }; };//back reel
		for(auto i : originalReelSide)
		{
			Vector temp = i + reelPosition;
			allOriginalCameraVerts.push_back(temp);
			temp.Y *= -1;
			allOriginalCameraVerts.push_back(temp);
		}
	}
	//Lens (16 verts): 70-85
	for(auto originalLen : originalLens)
	{
		Vector lensPosition = {28.772f, 0.f, -1.44f};
		Vector temp = lensPosition + originalLen;
		allOriginalCameraVerts.push_back(temp);
		Vector temp2 = originalLen;
		temp2.X *= -1;
		temp = lensPosition + temp2;
		allOriginalCameraVerts.push_back(temp);
	}
}

void RT::VisualCamera::TransformCamera(Vector location, Rotator rotation, float scale)
{
	allCalculatedCameraVerts.clear();
	Quat quat = RotatorToQuat(rotation);
	for(Vector calculatedVert : allOriginalCameraVerts)
	{
		calculatedVert.X -= 36;//adjust position to center the rotation around the lens
		calculatedVert.Z += 3.76126f/2;//for some reason the camera's Z is wrong and the lens isn't centered on the frustum
		calculatedVert = RotateVectorWithQuat(calculatedVert, quat);
		calculatedVert = calculatedVert * scale;
		calculatedVert = calculatedVert + location;
		allCalculatedCameraVerts.push_back(calculatedVert);
	}
}

void RT::VisualCamera::DrawObject(CanvasWrapper canvas, size_t startingIndex, size_t range)
{
	for(size_t i = 0; i != range; i += 2)
	{
		if(i == range - 2)
		{
			canvas.DrawLine(canvas.Project(allCalculatedCameraVerts[startingIndex + range-1]), canvas.Project(allCalculatedCameraVerts[startingIndex+1]));
			canvas.DrawLine(canvas.Project(allCalculatedCameraVerts[startingIndex + range-2]), canvas.Project(allCalculatedCameraVerts[startingIndex]));
		}
		else
		{
			canvas.DrawLine(canvas.Project(allCalculatedCameraVerts[startingIndex + i]), canvas.Project(allCalculatedCameraVerts[startingIndex + i + 2]));
			canvas.DrawLine(canvas.Project(allCalculatedCameraVerts[startingIndex + i + 1]), canvas.Project(allCalculatedCameraVerts[startingIndex + i + 3]));
		}
		canvas.DrawLine(canvas.Project(allCalculatedCameraVerts[startingIndex + i]), canvas.Project(allCalculatedCameraVerts[startingIndex + i + 1]));
	}
}

void RT::VisualCamera::DrawCamera(CanvasWrapper canvas, Vector location, Rotator rotation, float scale, LinearColor color)
{
	TransformCamera(location, rotation, scale);
    LinearColor inColor = canvas.GetColor();
	canvas.SetColor(color);

	//DRAW LINES
	std::vector<int> objectRanges;//Number of vertices per object
	objectRanges.push_back(8);//Matte box
	objectRanges.push_back(14);//Body
	objectRanges.push_back(24);//Reel 1
	objectRanges.push_back(24);//Reel 2
	objectRanges.push_back(16);//Lens
	int lineIndex = 0;

	for(int objectRange : objectRanges)
	{
		DrawObject(canvas, lineIndex, objectRange);
		lineIndex += objectRange;
	}

    canvas.SetColor(inColor);
}

void RT::VisualCamera::DrawCameraWithViewFrustum(CanvasWrapper canvas, Vector location, Rotator rotation, float FOV, float clipDistance, float scale, LinearColor color)
{
	//Draw camera object
	DrawCamera(canvas, location, rotation, scale, color);
	
	//Create frustum for visual camera
    float resX = static_cast<float>(canvas.GetSize().X);
    float resY = static_cast<float>(canvas.GetSize().Y);
	float ratio = resX / resY;

	RT::Frustum frustum(canvas, RotatorToQuat(rotation), location, FOV, 0, clipDistance);

	//Draw view frustum lines using 8 points of frustum
	//<F/N> = Front / Near
	//<T/B> = Top / Bottom
	//<L/R> = Left / Right
	constexpr int FTL = 0;
	constexpr int FTR = 1;
	constexpr int FBR = 2;
	constexpr int FBL = 3;
	constexpr int NTL = 4;
	constexpr int NTR = 5;
	constexpr int NBR = 6;
	constexpr int NBL = 7;

	//Far plane
    canvas.SetColor(LinearColor{0,255,0,255});
	canvas.DrawLine(canvas.Project(frustum.points[FTL]), canvas.Project(frustum.points[FTR]));//top line is green to indicate orientation
	canvas.SetColor(color);
	canvas.DrawLine(canvas.Project(frustum.points[FTR]), canvas.Project(frustum.points[FBR]));
	canvas.DrawLine(canvas.Project(frustum.points[FBR]), canvas.Project(frustum.points[FBL]));
	canvas.DrawLine(canvas.Project(frustum.points[FBL]), canvas.Project(frustum.points[FTL]));
	//Near plane
	canvas.DrawLine(canvas.Project(frustum.points[NTL]), canvas.Project(frustum.points[NTR]));
	canvas.DrawLine(canvas.Project(frustum.points[NTR]), canvas.Project(frustum.points[NBR]));
	canvas.DrawLine(canvas.Project(frustum.points[NBR]), canvas.Project(frustum.points[NBL]));
	canvas.DrawLine(canvas.Project(frustum.points[NBL]), canvas.Project(frustum.points[NTL]));
	//Lines between
	canvas.DrawLine(canvas.Project(frustum.points[FTL]), canvas.Project(frustum.points[NTL]));
	canvas.DrawLine(canvas.Project(frustum.points[FTR]), canvas.Project(frustum.points[NTR]));
	canvas.DrawLine(canvas.Project(frustum.points[FBR]), canvas.Project(frustum.points[NBR]));
	canvas.DrawLine(canvas.Project(frustum.points[FBL]), canvas.Project(frustum.points[NBL]));
}
