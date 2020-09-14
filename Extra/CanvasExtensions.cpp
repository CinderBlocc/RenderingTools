#include "CanvasExtensions.h"
#include "../Objects/Matrix3.h"
#include "../Objects/Frustum.h"
#include "bakkesmod/wrappers/canvaswrapper.h"
#include "bakkesmod/wrappers/GameObject/CameraWrapper.h"

LinearColor RT::GetPercentageColor(float percent, float alpha)
{
	LinearColor color = {0, 0, 0, 255*alpha};
	if(percent <= .5f)
	{
		color.R = 255;
		color.G = 0 + (255 * (percent*2));
	}
	else
	{
		color.R = 255 - (255 * (percent*2));
		color.G = 255;
	}

	return color;
}

float RT::GetVisualDistance(CanvasWrapper canvas, Frustum frustum, CameraWrapper camera, Vector objectLocation)
{
	//Concept: project a line of set 3D length to the 2D canvas.
	//Calculate the length of that 2D line to get perceived distance instead of linear distance

	Quat camQuat = RotatorToQuat(camera.GetRotation());
	Vector camUp = Matrix3(camQuat).up;

	float testScalePerspectiveLineLength = 100;
	Vector2F perspScaleLineStartProjected = canvas.ProjectF(objectLocation);
	Vector2F perspScaleLineEndProjected;
	Vector testScalePerspective = objectLocation + (camUp * testScalePerspectiveLineLength);

	//check if scale projection is within the top plane of the frustum, if not then invert the scale test line
	Vector planeNormal = {frustum.planes[0].x, frustum.planes[0].y, frustum.planes[0].z}; 
	if(Vector::dot(testScalePerspective, planeNormal) + frustum.planes[0].d + 1.0f > 0)
		perspScaleLineEndProjected = canvas.ProjectF(objectLocation + (camUp * testScalePerspectiveLineLength));
	else
		perspScaleLineEndProjected = canvas.ProjectF(objectLocation - (camUp * testScalePerspectiveLineLength));

	Vector2F perspScaleLine = {perspScaleLineEndProjected.X - perspScaleLineStartProjected.X, perspScaleLineEndProjected.Y - perspScaleLineStartProjected.Y};
	float perspScale = sqrtf(perspScaleLine.X * perspScaleLine.X + perspScaleLine.Y * perspScaleLine.Y);
	//if(perspScale > 100)
	//	perspScale = 100;
	float distancePercentage = perspScale/100;//1 is close, 0 is infinitely far away
	//if(distancePercentage > 1)
	//	distancePercentage = 1;
	if(distancePercentage < 0)
		distancePercentage = 0;

	return distancePercentage;
}

void RT::SetColor(CanvasWrapper canvas, std::string colorName, float opacity)
{
	LinearColor color = {0,0,0,opacity};

	     if(colorName == "black")   color = LinearColor{0,0,0,opacity};
	else if(colorName == "white")   color = LinearColor{255,255,255,opacity};
	else if(colorName == "red")     color = LinearColor{255,0,0,opacity};
	else if(colorName == "green")   color = LinearColor{0,255,0,opacity};
	else if(colorName == "blue")    color = LinearColor{0,0,255,opacity};
	else if(colorName == "yellow")  color = LinearColor{255,255,0,opacity};
	else if(colorName == "cyan")    color = LinearColor{0,255,255,opacity};

	canvas.SetColor(color);
}

void RT::DrawDebugStrings(CanvasWrapper canvas, const std::vector<DebugString>& drawStrings, EDebugStringBackground background, int minWidth)
{
	if(drawStrings.empty()) return;

	Vector2 base = {50,50};
	if(background > EDebugStringBackground::BG_None)
	{
        //If background type is static, leave finalWidth as minWidth
        int finalWidth = minWidth;

        //Determine dynamic width of background based on longest string
        if(background > EDebugStringBackground::BG_StaticWidth)
        {
            for(const auto& str : drawStrings)
            {
                int strSize = static_cast<int>(canvas.GetStringSize(str.Text).X);
                if(strSize > minWidth)
                    finalWidth = strSize;
            }
        }

        //Give a margin at the right of the string to match the left margin
        finalWidth += 20;

        //Draw the background
        canvas.SetColor(LinearColor{0,0,0,150});
		canvas.SetPosition(base - Vector2{10,10});
		canvas.FillBox(Vector2{finalWidth, static_cast<int>(20 * (drawStrings.size() + 1))});
	}

    for(const auto& str : drawStrings)
    {
        canvas.SetPosition(base);
        canvas.SetColor(str.Color);
        canvas.DrawString(str.Text);
        base.Y += 20;
    }
}
