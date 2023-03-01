#include "MenuBox.h"
#include "RectTool.h"

using namespace RectTool;

MenuBox::MenuBox(SDL_Texture* spriteSheet, Json::Value& data) :
	UIElement{ spriteSheet, GenerateRect(data["Offset"]["RectData"]),GenerateRect(data["ScreenPosition"]["RectData"]), data["Name"].asString()}
{
}

void MenuBox::ToggleActive(bool state)
{
	if (isActive != state)
	{
		isActive = state;
	}
}

bool MenuBox::GetActiveStatus()
{
	return isActive;
}
