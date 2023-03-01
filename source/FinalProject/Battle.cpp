#include "Battle.h"

void Battle::AddAction(std::string action)
{
	battleActions.emplace_back(action);
}

void Battle::GenerateAction()
{
	++turnCount;
	switch (battleActions.size())
	{
	case 2:
		actionQueue.emplace_back(battleActions.at(0), battleActions.at(1));
		break;
	case 3:
		actionQueue.emplace_back(battleActions.at(0), battleActions.at(1), battleActions.at(2));
		break;
	default:
		actionQueue.emplace_back("Dead","Dead","Dead");
		break;
	}
	battleActions.clear();
}
