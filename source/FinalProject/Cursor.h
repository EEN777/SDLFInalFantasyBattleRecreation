#pragma once
#include <SDL2/SDL.h>
#include <json/json.h>
#include <unordered_map>
#include "UIElement.h"
#include "Context.h"
#include "Battle.h"

class Cursor : public UIElement
{
	std::unordered_map<std::string, Context> contexts;
	std::unordered_map<std::string, Context> ownedContexts;
	std::vector<std::string> party;
	std::string currentPosition{ "Attack" };
	bool isActive{ false };
	const SDL_Rect visibleOffset{ offset };
	const SDL_Rect invisibleOffset{ 0,0,0,0 };
	Battle& battle;
	std::string currentContext{ "Main" };
public:
	Cursor(SDL_Texture*,Json::Value&,Json::Value&,Battle&);
	void MoveUp();
	void MoveDown();
	void MoveRight();
	void MoveLeft();
	void ToggleActive(bool);
	void ContextSwitch(std::string);
	void Select();
	std::string& GetContext();
	bool CheckIfSubMenu();
};