#include "UIElement.h"

UIElement::UIElement(SDL_Texture* newTexture, SDL_Rect newOffset, SDL_Rect newPosition, const std::string& characterName) :
	spriteSheet{ newTexture }, offset{ newOffset }, screenPosition{ newPosition }, name{ characterName }
{
}

const SDL_Texture* UIElement::GetTexture()
{
	return spriteSheet;
}

const std::string& UIElement::GetName()
{
	return name;
}

void UIElement::Draw(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, spriteSheet, &offset, &screenPosition);
}