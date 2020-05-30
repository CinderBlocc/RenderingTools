#include "RenderingExamples.h"
#include "bakkesmod\wrappers\includes.h"

BAKKESMOD_PLUGIN(RenderingExamples, "Rendering Examples plugin to showcase RenderingTools", "1.0", PLUGINTYPE_FREEPLAY)


void RenderingExamples::onLoad()
{
	gameWrapper->RegisterDrawable(bind(&RenderingExamples::Render, this, std::placeholders::_1));
}
void RenderingExamples::onUnload() {}

void RenderingExamples::Render(CanvasWrapper canvas)
{
	//I will fill this with examples in due time
}