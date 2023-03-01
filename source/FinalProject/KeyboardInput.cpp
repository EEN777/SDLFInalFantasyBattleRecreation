#include "KeyboardInput.h"

PlayerInput::PlayerActions KeyboardInput::GetPlayerInput()
{
	SDL_PollEvent(&input);
	PlayerActions action{Default};
	if (input.type != SDL_KEYDOWN)
	{
		return Default;
	}

	switch (input.key.keysym.sym)
	{
	case SDLK_UP:
		action = MoveUp;
		break;
	case SDLK_DOWN:
		action = MoveDown;
		break;
	case SDLK_LEFT:
		action = MoveLeft;
		break;
	case SDLK_RIGHT:
		action = MoveRight;
		break;
	case SDLK_ESCAPE:
		action = Quit;
		break;
	case SDLK_RETURN:
		action = Select;
		break;
	case SDLK_0:
		action = Debug;
		break;
	default:
		action = Default;
		break;

	}

	return action;
}
