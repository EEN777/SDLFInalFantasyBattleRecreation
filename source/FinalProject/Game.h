#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <json/json.h>
#include "PartyMember.h"
#include "Enemy.h"
#include "UIElement.h"
#include "Cursor.h"
#include "Battle.h"
#include "MenuBox.h"


struct Game
{
	const int screenWidth{ 1024 };
	const int screenHeight{ 960 };

	const Uint64 frameCap{ 17 };

	SDL_Window* window{ nullptr };
	SDL_Renderer* renderer{ nullptr };

	SDL_Rect resultRect{ 654,710,87,74 };

	TTF_Font* font{ nullptr };
	Mix_Music* battleTheme{ nullptr };
	Mix_Music* victoryTheme{ nullptr };
	Mix_Music* gameOverTheme{ nullptr };

	SDL_Texture* resultText{ nullptr };

	Mix_Chunk* hitSfx{ nullptr };
	Mix_Chunk* missSfx{ nullptr };
	Mix_Chunk* restoreSfx{ nullptr };
	Mix_Chunk* blackMagicSfx{ nullptr };

	std::vector<Cursor> cursors;
	
	Battle battle{};

	std::string currentPartyMember{"Cloud"};

	bool displayingResult{ false };

	std::unordered_map<std::string, SDL_Texture*> spriteSheets;
	std::unordered_map<std::string, PartyMember> partyMembers;
	std::unordered_map<std::string, Enemy> enemies;
	std::unordered_map<std::string, MenuBox> menus;
	std::unordered_map<std::string, MenuBox> magicMenus;
	std::unordered_map<std::string, MenuBox> actorPanels;
	std::unordered_map<std::string, MenuBox> subjectPanels;
	std::unordered_map<std::string, MenuBox> actionPanels;
	std::unordered_map<std::string, MenuBox> resultPanels;
	std::unordered_map<std::string, MenuBox> partyWeapons;
	std::unordered_map<std::string, int> spellStrength;

	std::vector<SDL_Texture*> spritesheets;

	void CreateResult(int);
	void CheckResult();

	Game();
	~Game();
};