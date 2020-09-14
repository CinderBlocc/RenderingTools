#include "bakkesmod/wrappers/canvaswrapper.h"
#include "Circle2D.h"
#include <vector>

RT::Circle2D::Circle2D()
    : location({0,0}), radius(20), steps(16), lineThickness(1) {}
RT::Circle2D::Circle2D(Vector2F loc, float rad)
    : location(loc), radius(rad), steps(16), lineThickness(1) {}
RT::Circle2D::Circle2D(Vector2F loc, float rad, int step)
    : location(loc), radius(rad), steps(step), lineThickness(1) {}
RT::Circle2D::Circle2D(Vector2F loc, float rad, int step, float lineThicc)
    : location(loc), radius(rad), steps(step), lineThickness(lineThicc) {}

void RT::Circle2D::Draw(CanvasWrapper canvas) const
{
    std::vector<Vector2F> circlePoints;

    //Generate points
    for(int i = 0; i < steps; ++i)
    {
        float perc = static_cast<float>(i) / steps;
        float X = -sinf(CONST_PI_F * 2.f * perc);
        float Y = cosf(CONST_PI_F * 2.f * perc);

        X *= radius;
        Y *= radius;

        X += location.X;
        Y += location.Y;

        circlePoints.emplace_back(Vector2F{X, Y});
    }

    //Draw lines
    Vector2 canvasSize = canvas.GetSize();
    for(size_t i = 0; i != circlePoints.size(); ++i)
    {
        Vector2F currentPoint = circlePoints[i];
        Vector2F nextPoint;
        if(i + 1 == circlePoints.size())
        {
            nextPoint = circlePoints[0];
        }
        else
        {
            nextPoint = circlePoints[i + 1];
        }

        bool isCurrentPointOnScreen = currentPoint.X >= 0 && currentPoint.Y >= 0 && currentPoint.X <= canvasSize.X && currentPoint.Y <= canvasSize.Y;
        bool isNextPointOnScreen = nextPoint.X >= 0 && nextPoint.Y >= 0 && nextPoint.X <= canvasSize.X && nextPoint.Y <= canvasSize.Y;

        if(isCurrentPointOnScreen || isNextPointOnScreen)
        {
            if(lineThickness == 1)
            {
                canvas.DrawLine(currentPoint, nextPoint);
            }
            else
            {
                canvas.DrawLine(currentPoint, nextPoint, lineThickness);
            }
        }
    }
}
