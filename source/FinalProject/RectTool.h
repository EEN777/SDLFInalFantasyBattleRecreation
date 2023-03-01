#pragma once
#include <SDL2/SDL.h>
#include <json/json.h>

namespace RectTool {
inline SDL_Rect GenerateRect(Json::Value& container)
{
	return SDL_Rect{ container[0].asInt(),container[1].asInt(),container[2].asInt(),container[3].asInt() };
}

}