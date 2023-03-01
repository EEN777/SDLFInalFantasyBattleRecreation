#include "Character.h"

Character::Character(SDL_Texture* newTexture, SDL_Rect newOffset, SDL_Rect newPosition, const std::string& characterName) :
	spriteSheet{ newTexture }, offset{ newOffset }, screenPosition{ newPosition }, name{characterName}
{
}

const SDL_Texture* Character::GetTexture()
{
	return spriteSheet;
}

const std::string& Character::GetName()
{
	return name;
}

void Character::Draw(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, spriteSheet, &offset, &screenPosition);
}
