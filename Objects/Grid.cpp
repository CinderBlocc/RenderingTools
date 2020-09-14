#include "bakkesmod/wrappers/canvaswrapper.h"
#include "Grid.h"
#include "Matrix3.h"
#include "Frustum.h"
#include "Line.h"

RT::Grid::Grid()
	: width(100), height(100), widthSegs(10), heightSegs(10) {}

RT::Grid::Grid(Vector loc, Quat rot, float w, float h, int wSegs, int hSegs)
	: location(loc), orientation(rot), width(w), height(h), widthSegs(wSegs), heightSegs(hSegs) {}

void RT::Grid::Draw(CanvasWrapper canvas, Frustum &frustum, bool useThickMidline) const
{
	//The terms "horizontal" and "vertical" refer to the direction the line is drawn

	int tempWidthSegs = (widthSegs < 2) ? 2 : widthSegs;
	int tempHeightSegs = (heightSegs < 2) ? 2 : heightSegs;

	bool drawHorizontalCenterLine = tempHeightSegs % 2 == 0;
	bool drawVerticalCenterLine = tempWidthSegs % 2 == 0;

	float horizontalGridSpacing = height / tempHeightSegs;
	float verticalGridSpacing = width / tempWidthSegs;
	float horizontalStartVal = height * .5f;
	float verticalStartVal = width * .5f;
	
	RT::Matrix3 mat(orientation);
	Vector startLocation = location - (mat.right * verticalStartVal) - (mat.up * horizontalStartVal);

	//Draw horizontal lines
	for(int y = 1; y < tempHeightSegs; ++y)
	{
		Line line;
		line.lineBegin = startLocation + (mat.up * verticalGridSpacing * static_cast<float>(y));
		line.lineEnd = line.lineBegin + (mat.right * width);
		if(useThickMidline && drawHorizontalCenterLine && y == tempHeightSegs/2)
		{
			line.thickness = 3;
		}
		line.DrawWithinFrustum(canvas, frustum);
	}

	//Draw horizontal lines
	for(int x = 1; x < tempWidthSegs; ++x)
	{
		Line line;
		line.lineBegin = startLocation + (mat.right * horizontalGridSpacing * static_cast<float>(x));
		line.lineEnd = line.lineBegin + (mat.up * height);
		if(useThickMidline && drawVerticalCenterLine && x == tempWidthSegs/2)
		{
			line.thickness = 3;
		}
		line.DrawWithinFrustum(canvas, frustum);
	}

	//Draw outer lines

	Vector corners[4];
	corners[0] = startLocation;
	corners[1] = startLocation + (mat.right * width);
	corners[2] = startLocation + (mat.right * width) + (mat.up * height);
	corners[3] = startLocation + (mat.up * height);

	Line(corners[0], corners[1]).DrawWithinFrustum(canvas, frustum);
	Line(corners[1], corners[2]).DrawWithinFrustum(canvas, frustum);
	Line(corners[2], corners[3]).DrawWithinFrustum(canvas, frustum);
	Line(corners[3], corners[0]).DrawWithinFrustum(canvas, frustum);

	/*
	Vector2F corners[4];
	corners[0] = canvas.ProjectF(startLocation);
	corners[1] = canvas.ProjectF(startLocation + (mat.right * width));
	corners[2] = canvas.ProjectF(startLocation + (mat.right * width) + (mat.up * height));
	corners[3] = canvas.ProjectF(startLocation + (mat.up * height));

	canvas.DrawLine(corners[0], corners[1]);
	canvas.DrawLine(corners[1], corners[2]);
	canvas.DrawLine(corners[2], corners[3]);
	canvas.DrawLine(corners[3], corners[0]);
	*/
}
