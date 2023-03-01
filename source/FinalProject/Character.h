#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class Character
{
	SDL_Texture* spriteSheet{nullptr};
	const std::string name;

public:
	Character() = default;
	Character(SDL_Texture*, SDL_Rect, SDL_Rect,const std::string&);

	SDL_Rect screenPosition;
	SDL_Rect offset;

	const SDL_Texture* GetTexture();

	const std::string& GetName();

	void Draw(SDL_Renderer*);
};
	

