#include "Game.h"
#include "RectTool.h"
#include <fstream>
#include <cassert>

void Game::CreateResult(int result)
{
	SDL_Color color = { 0xFF,0xFF,0xFF };
	SDL_Surface* surface = TTF_RenderText_Blended(font, std::to_string(result).c_str(), color);
	resultText = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
}

void Game::CheckResult()
{
	if (displayingResult)
	{
		SDL_RenderCopy(renderer, resultText, nullptr, &resultRect);
	}
}

Game::Game()
{
	using namespace Json;
	using namespace std;
	using namespace RectTool;

	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("Final Fantasy I - Recreation ", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0x0);

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	ifstream gameJson("Content/GameData.json");
	assert(gameJson);
	Value gameData;
	Reader reader;
	reader.parse(gameJson, gameData);
	auto images{ gameData["SpriteSheets"] };
	auto party{ gameData["PartyMembers"] };
	auto enemyList{ gameData["Enemies"] };
	auto windows{ gameData["Menus"] };
	auto actorWindows{ gameData["ActorPanels"] };
	auto subjectWindows{ gameData["SubjectPanels"] };
	auto actionWindows{ gameData["ActionPanels"] };
	auto resultWindows{ gameData["ResultPanels"] };
	auto magicWindows{ gameData["MagicMenus"] };
	auto spells{ gameData["SpellStrength"] };
	auto weapons{ gameData["Weapons"] };

	battleTheme = Mix_LoadMUS("Content/Battle.wav");
	victoryTheme = Mix_LoadMUS("Content/Victory.wav");
	gameOverTheme = Mix_LoadMUS("Content/GameOver.wav");
	Mix_PlayMusic(battleTheme, -1);

	hitSfx = Mix_LoadWAV("Content/Hit.wav");
	missSfx = Mix_LoadWAV("Content/Miss.wav");
	restoreSfx = Mix_LoadWAV("Content/Restorative.wav");
	blackMagicSfx = Mix_LoadWAV("Content/BlackMagic.wav");

	font = TTF_OpenFont("Content/FFFont.ttf", 25);
	std::cout << TTF_GetError();

	for (auto& image : images)
	{
		if (image["Name"].asString() == "Menus" || image["Name"].asString() == "MagicMenus")
		{
			SDL_Surface* loadedSurface{ IMG_Load(image["Filepath"].asCString()) };
			spriteSheets.emplace(image["Name"].asString(), SDL_CreateTextureFromSurface(renderer, loadedSurface));
			SDL_FreeSurface(loadedSurface);
		}
		else
		{
			SDL_Surface* loadedSurface{ IMG_Load(image["Filepath"].asCString()) };
			SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0, 0));
			spriteSheets.emplace(image["Name"].asString(),SDL_CreateTextureFromSurface(renderer,loadedSurface));
			SDL_FreeSurface(loadedSurface);
		}
	}

	for (auto& member : party)
	{
		std::string sheet{member["SpriteSheet"].asString()};
		partyMembers.emplace(member["Name"].asString(),PartyMember{spriteSheets.at(sheet),member});
	}

	for (auto& enemy : enemyList)
	{
		std::string sheet{ enemy["SpriteSheet"].asString() };
		enemies.emplace(enemy["Name"].asString(),Enemy(spriteSheets.at(sheet), enemy));
		++battle.enemyTurnCount;
	}

	for (auto& menu : windows)
	{
		std::string sheet{menu["SpriteSheet"].asString()};
		menus.emplace(menu["Name"].asString(), MenuBox(spriteSheets.at(sheet), menu));
	}

	for (auto& actorPanel : actorWindows)
	{
		std::string sheet{actorPanel["SpriteSheet"].asString()};
		actorPanels.emplace(actorPanel["Name"].asString(), MenuBox(spriteSheets.at(sheet), actorPanel));
	}

	for (auto& subjectPanel : subjectWindows)
	{
		std::string sheet{ subjectPanel["SpriteSheet"].asString() };
		subjectPanels.emplace(subjectPanel["Name"].asString(), MenuBox(spriteSheets.at(sheet), subjectPanel));
	}

	for (auto& actionPanel : actionWindows)
	{
		std::string sheet{ actionPanel["SpriteSheet"].asString() };
		actionPanels.emplace(actionPanel["Name"].asString(), MenuBox(spriteSheets.at(sheet), actionPanel));
	}

	for (auto& resultPanel : resultWindows)
	{
		std::string sheet{ resultPanel["SpriteSheet"].asString() };
		resultPanels.emplace(resultPanel["Name"].asString(), MenuBox(spriteSheets.at(sheet), resultPanel));
	}

	for (auto& magicMenu : magicWindows)
	{
		std::string sheet{ magicMenu["SpriteSheet"].asString() };
		magicMenus.emplace(magicMenu["Name"].asString(), MenuBox(spriteSheets.at(sheet), magicMenu));
	}

	for (auto& spell : spells)
	{
		spellStrength.emplace(spell["Spell"].asString(), spell["Power"].asInt());
	}

	for (auto& weapon : weapons)
	{
		std::string sheet{ weapon["SpriteSheet"].asString() };
		partyWeapons.emplace(weapon["Name"].asString(), MenuBox( spriteSheets.at(sheet), weapon ));
	}

	{
		std::string sheet{ gameData["Cursor"][0]["SpriteSheet"].asString() };
		cursors.emplace_back(Cursor(spriteSheets.at(sheet), gameData["Cursor"][0],gameData,battle));
	}

}

Game::~Game()
{
	for (auto& texture : spriteSheets)
	{
		SDL_DestroyTexture(texture.second);
	}

	SDL_DestroyTexture(resultText);

	Mix_FreeChunk(hitSfx);
	Mix_FreeChunk(missSfx);
	Mix_FreeChunk(restoreSfx);
	Mix_FreeChunk(blackMagicSfx);

	Mix_FreeMusic(battleTheme);
	Mix_FreeMusic(victoryTheme);
	Mix_FreeMusic(gameOverTheme);

	TTF_CloseFont(font);

	Mix_CloseAudio();


	SDL_assert(window != nullptr);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	window = nullptr;
	renderer = nullptr;
	SDL_TLSCleanup();
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}
