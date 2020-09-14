#pragma once
#include "bakkesmod/wrappers/wrapperstructs.h"

class CanvasWrapper;

namespace RT
{
    class Circle2D
    {
    public:
        Vector2F location;
        float radius;
        int steps;
        float lineThickness;

        // CONSTRUCTORS
        Circle2D();
        Circle2D(Vector2F loc, float rad);
        Circle2D(Vector2F loc, float rad, int step);
        Circle2D(Vector2F loc, float rad, int step, float lineThicc);

        // FUNCTIONS
		void Draw(CanvasWrapper canvas) const;
    };
}
