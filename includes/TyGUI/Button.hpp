#pragma once

//defines a button element

#include <imgui.h>
#include <cstring>

namespace TyGUI::Widget::Element
{
	//creates a button that matches the needed size
	inline bool Button(const char* text, const float baseWidthMultiplyer = 12.0f, const float height = 25.0f)
	{
		return ImGui::Button(text, ImVec2(strlen(text) * baseWidthMultiplyer, height));
	}
}