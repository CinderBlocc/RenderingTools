#pragma once
#pragma comment(lib, "BakkesMod.lib")
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "../../RenderingTools.h"

class RenderingExamples : public BakkesMod::Plugin::BakkesModPlugin
{
private:
	RT::RenderingAssistant RA;

public:
	virtual void onLoad();
	virtual void onUnload();

	void Render(CanvasWrapper canvas);
};
