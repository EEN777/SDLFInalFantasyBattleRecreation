#pragma once
#include <map>
#include <json/json.h>
#include <SDL2/SDL.h>
#include "Neighbors.h"

struct Context 
{
	std::map<std::string, SDL_Rect> positions;
	std::map<std::string, Neighbors> neighborList;
	const bool isFinalContext;
	const bool isSubMenu;
	const bool isOwned;
	const std::string startingLocation;

	Context(Json::Value&);
};
