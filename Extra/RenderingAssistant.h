#pragma once
#include "../Objects/Frustum.h"

//RECOMMENDATIONS:
//An instance of this class should be added as a member variable to your plugin class so you can use it in any function
//Update the frustum once per tick (in a master Drawable function), and use it to cull all objects

namespace RT
{
	class RenderingAssistant
	{
	public:
		Frustum frustum;
	};
}
