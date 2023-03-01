#pragma once
#include "Game.h"
#include "PartyMember.h"
#include "Enemy.h"

class TurnHandler
{
	Game& game;
	int currentTurn{0};
	int previousActionListSize{ 0 };
	bool isHandlingActions{ false };
	bool isAcceptingCommands{true};
	bool isBattleStarted{ false };
	bool isStartingTurn{ false };
	bool isEndingTurn{ false };
	std::vector<std::string> party;
	std::unordered_map<std::string, Action> actionOwners{};
public:
	TurnHandler(Game&);

	void StartBattle();

	void InterperetActions();
	bool GetStatus();
	void SetStatus(bool);
	void Clear();
	std::string& GetPartyMember(int);
};