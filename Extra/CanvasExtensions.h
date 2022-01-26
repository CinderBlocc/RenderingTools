#pragma once
#include "bakkesmod/wrappers/wrapperstructs.h"
#include <string>
#include <vector>

class CanvasWrapper;
class CameraWrapper;

namespace RT
{
    class Frustum;

    struct DebugString
    {
        std::string Text;
        LinearColor Color = {0.0f, 255.0f, 0.0f, 255.0f};

        DebugString(std::string InText) : Text(InText) {}
        DebugString(std::string InText, LinearColor InColor) : Text(InText), Color(InColor) {}
    };

    enum class EDebugStringBackground
    {
        BG_None = 0,           //No background
        BG_StaticWidth,        //Background. Width defined manually
        BG_PureDynamicWidth,   //Background. Width defined purely by width of longest string
        BG_DynamicWidthMinimum //Background. Width defined by width of longest string, or by specified minimum width
    };

    LinearColor GetPercentageColor(float percent, float alpha = 1.0f);
	float GetVisualDistance(CanvasWrapper canvas, Frustum frustum, CameraWrapper camera, Vector objectLocation);
	void SetColor(CanvasWrapper canvas, std::string colorName, float opacity = 255.0f);
	void DrawDebugStrings(CanvasWrapper canvas, const std::vector<DebugString>& drawStrings, EDebugStringBackground background=EDebugStringBackground::BG_None, int32_t minWidth = 200.0f);
}
