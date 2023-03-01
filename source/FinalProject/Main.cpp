#define SDL_MAIN_HANDLED
// Required Libraries:
// sdl2, sdl2-image, sdl2-ttf, sdl2-mixer, and jsoncpp installed through vcpkg.

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include <crtdbg.h>
#include <cassert>
#include "Game.h"
#include "PartyMember.h"
#include "KeyboardInput.h"
#include "TurnHandler.h"
#include "SequencePlayer.h"

void HandleInput(KeyboardInput&,Game&,bool&);
void RenderFrame(Game&);

SDL_Rect scanlineRect{ 0,0,1024,960 };
SDL_Rect scanlineRect2{ 0,-960,1024,960 };

int main()
{
#if defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	bool quit = false;
	bool isInEndgame = false;
	KeyboardInput keyboardInput;
	Game game;
	TurnHandler turnHandler{ game };
	SequencePlayer sequencePlayer{ game,turnHandler };

	game.cursors.at(0).ToggleActive(false);


	while (!quit)
	{
		if (isInEndgame)
		{
			HandleInput(keyboardInput, game, quit);
			RenderFrame(game);
			game.cursors.at(0).ToggleActive(false);
			game.menus.at("BattleCanvas").ToggleActive(false);
		}

		else
		{
			HandleInput(keyboardInput, game, quit);
			RenderFrame(game);
			if (!turnHandler.GetStatus())
			{
				turnHandler.StartBattle();
			}
			else
			{
				sequencePlayer.PlaySequence();
				if (sequencePlayer.isGameComplete)
				{
					if (sequencePlayer.partyLost)
					{
						isInEndgame = true;
						Mix_PlayMusic(game.gameOverTheme, -1);
					}
					else
					{
						isInEndgame = true;
						Mix_PlayMusic(game.victoryTheme, -1);
						for (auto& member : game.partyMembers)
						{
							member.second.action = PartyMember::Victory;
						}
					}
				}
			}
		}
	}
	return 0;
}

void HandleInput(KeyboardInput& keyboardInput,Game& game,bool& quit)
{
	switch (keyboardInput.GetPlayerInput())
	{
	case PlayerInput::MoveUp:
		game.cursors.at(0).MoveUp();
		break;
	case PlayerInput::MoveDown:
		game.cursors.at(0).MoveDown();
		break;
	case PlayerInput::MoveLeft:
		game.cursors.at(0).MoveLeft();
		break;
	case PlayerInput::MoveRight:
		game.cursors.at(0).MoveRight();
		break;
	case PlayerInput::Debug:
		game.partyMembers.at("Vivi").TakeDamage(1000);
		game.partyMembers.at("Al").TakeDamage(1000);
		break;
	case PlayerInput::Select:
		game.cursors.at(0).Select();
		if (game.cursors.at(0).CheckIfSubMenu())
		{
			game.magicMenus.at(game.currentPartyMember).ToggleActive(true);
		}
		else
		{
			game.magicMenus.at(game.currentPartyMember).ToggleActive(false);
		}
		break;
	case PlayerInput::Quit:
		quit = true;
		break;
	default:
		break;
	}

}

void RenderFrame(Game& game)
{
	auto frameBegin = SDL_GetTicks64();
	SDL_RenderClear(game.renderer);
	SDL_RenderCopy(game.renderer, game.spriteSheets.at("Background"), nullptr, nullptr);
	for (auto& member : game.partyMembers)
	{
		member.second.EventTick();
		member.second.Draw(game.renderer);
		assert(game.font != nullptr);
		SDL_Color color = { 0xFF,0xFF,0xFF };
		SDL_Surface* surface = TTF_RenderText_Blended(game.font, std::to_string(member.second.GetHealth()).c_str(), color);
		SDL_Texture* texture = SDL_CreateTextureFromSurface(game.renderer, surface);
		if ((member.second.GetHealth() / 100) == 0)
		{

			if ((member.second.GetHealth() / 10) == 0) 
			{
				SDL_Rect newRect = member.second.healthPosition;
				newRect.w -= 68;
				SDL_RenderCopy(game.renderer, texture, nullptr, &newRect);
			}
			else
			{
				SDL_Rect newRect = member.second.healthPosition;
				newRect.w -= 34;
				SDL_RenderCopy(game.renderer, texture, nullptr, &newRect);
			}
		}
		else
		{
			SDL_RenderCopy(game.renderer, texture, nullptr, &member.second.healthPosition);
		}
		SDL_FreeSurface(surface);
		SDL_DestroyTexture(texture);
	}

	for (auto& enemy : game.enemies)
	{
		enemy.second.Draw(game.renderer);
	}

	for (auto& menu : game.menus)
	{
		if (menu.second.GetActiveStatus())
		{
			menu.second.Draw(game.renderer);
		}
	}

	for (auto& menu : game.magicMenus)
	{
		if (menu.second.GetActiveStatus())
		{
			menu.second.Draw(game.renderer);
		}
	}

	for (auto& menu : game.actorPanels)
	{
		if (menu.second.GetActiveStatus())
		{
			menu.second.Draw(game.renderer);
		}
	}

	for (auto& menu : game.subjectPanels)
	{
		if (menu.second.GetActiveStatus())
		{
			menu.second.Draw(game.renderer);
		}
	}

	for (auto& menu : game.actionPanels)
	{
		if (menu.second.GetActiveStatus())
		{
			menu.second.Draw(game.renderer);
		}
	}

	for (auto& menu : game.resultPanels)
	{
		if (menu.second.GetActiveStatus())
		{
			menu.second.Draw(game.renderer);
		}
	}

	for (auto& menu : game.partyWeapons)
	{
		if (menu.second.GetActiveStatus())
		{
			menu.second.Draw(game.renderer);
		}
	}
	game.CheckResult();
	game.cursors.at(0).Draw(game.renderer);
	SDL_RenderCopy(game.renderer, game.spriteSheets.at("Scanlines"), nullptr, &scanlineRect);
	SDL_RenderCopy(game.renderer, game.spriteSheets.at("Scanlines"), nullptr, &scanlineRect2);
	scanlineRect.y += 2;
	scanlineRect2.y += 2;

	if (scanlineRect.y == 960)
	{
		scanlineRect.y = -960;
	}

	if (scanlineRect2.y == 960)
	{
		scanlineRect2.y = -960;
	}

	SDL_RenderPresent(game.renderer);
	auto frameEnd = SDL_GetTicks64();

	if ((frameEnd - frameBegin) < game.frameCap)
	{
		SDL_Delay((Uint32)(game.frameCap - (frameEnd - frameBegin)));
	}
}
