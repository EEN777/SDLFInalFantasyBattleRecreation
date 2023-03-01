#pragma once
#include <vector>
#include <string>
#include "PartyMember.h"
#include "Enemy.h"
#include "Action.h"

class Battle
{
	std::vector<std::string> battleActions{};
	int currentTurn{};
public:
	std::vector<Action> actionQueue{};
	bool isTakingCommands{ false };
	int turnCount{0};
	int enemyTurnCount{};

	void AddAction(std::string);
	void GenerateAction();
};