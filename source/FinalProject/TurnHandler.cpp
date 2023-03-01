#include "TurnHandler.h"
#include <cassert>

TurnHandler::TurnHandler(Game& gameInstance):
	game{gameInstance}
{
	party.push_back("Cloud");
	party.push_back("Vivi");
	party.push_back("Aerith");
	party.push_back("Al");
}

void TurnHandler::StartBattle()
{
	if (game.partyMembers.at(party.at(currentTurn)).isDead)
	{
		game.battle.GenerateAction();
		isEndingTurn = true;
	}

	game.currentPartyMember = party.at(currentTurn);
	if (!isAcceptingCommands)
	{
		if (!isHandlingActions)
		{
			game.cursors.at(0).ToggleActive(false);
			isHandlingActions = true;
			return;
		}
	}
	if ((int)game.battle.actionQueue.size() == 4)
	{
		isAcceptingCommands = false;
	}
	if ((int)game.battle.actionQueue.size() > previousActionListSize)
	{
		isEndingTurn = true;
	}
	previousActionListSize = (int) game.battle.actionQueue.size();
	if (!isBattleStarted)
	{
		isBattleStarted = true;
		game.cursors.at(0).ToggleActive(true);
		for (auto& member : game.partyMembers)
		{
			party.emplace_back(member.first);
		}
		isStartingTurn = true;
	}
	if (isStartingTurn)
	{
		if (isAcceptingCommands)
		{
			game.partyMembers.at(party.at(currentTurn)).action = PartyMember::Walk;
			isStartingTurn = false;
		}
	}

	if (isEndingTurn)
	{
		game.partyMembers.at(party.at(currentTurn)).action = PartyMember::WalkBack;
		isEndingTurn = false;
		actionOwners.emplace(game.partyMembers.at(party.at(currentTurn)).GetName(), game.battle.actionQueue.at(currentTurn));
		++currentTurn;
		if (currentTurn > 3)
		{
			currentTurn = 0;
		}
		isStartingTurn = true;
	}
}

void TurnHandler::InterperetActions()
{
	for (auto& action : actionOwners)
	{
		if (action.second.first == "Attack")
		{
			game.enemies.at(action.second.second).TakeDamage(500);
			std::cout << action.second.second << " HP: " << game.enemies.at(action.second.second).GetHealth() << std::endl;
		}
		if (action.second.first == "Magic")
		{
			std::cout << action.first << " used Magic." << action.second.third << std::endl;  
		}

		if (action.second.first == "Item")
		{
			std::cout << action.first << " an Item." << std::endl;
		}
	}
}

bool TurnHandler::GetStatus()
{
	return isHandlingActions;
}

void TurnHandler::SetStatus(bool state)
{
	isHandlingActions = state;
	if (!isHandlingActions)
	{
		isAcceptingCommands = true;
	}
	else
	{
		isAcceptingCommands = false;
	}
}

void TurnHandler::Clear()
{
	actionOwners.clear();
}

std::string& TurnHandler::GetPartyMember(int placement)
{
	assert(placement < 4);
	return party.at(placement);
}