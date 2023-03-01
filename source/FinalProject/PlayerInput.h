#pragma once
#include <SDL2/SDL.h>
class PlayerInput
{
public:
	SDL_Event input{};
	enum PlayerActions
	{
		MoveUp,
		MoveDown,
		MoveLeft,
		MoveRight,
		Quit,
		Select,
		Debug,
		Default
	};

	virtual PlayerActions GetPlayerInput() = 0;
};

