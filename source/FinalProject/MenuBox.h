#pragma once
#include <json/json.h>
#include "UIElement.h"

class MenuBox : public UIElement
{
	bool isActive{ false };
public:
	MenuBox(SDL_Texture*, Json::Value&);
	void ToggleActive(bool);
	bool GetActiveStatus();
};

