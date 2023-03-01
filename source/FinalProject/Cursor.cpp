#include "Cursor.h"
#include "RectTool.h"
#include <cassert>

using namespace RectTool;

Cursor::Cursor(SDL_Texture* spriteSheet, Json::Value& data, Json::Value& contextData, Battle& instance) :
	UIElement{ spriteSheet, GenerateRect(data["Offset"]["RectData"]),GenerateRect(data["ScreenPosition"]["RectData"]), data["Name"].asString()},
	battle{instance}
{
	for (auto& context : contextData["MenuContexts"])
	{
		contexts.emplace(context["ContextName"].asString(), context);
	}

	for (auto& context : contextData["OwnedContexts"])
	{
		contexts.emplace(context["ContextName"].asString(), context);
	}

	party.push_back("Cloud");
	party.push_back("Vivi");
	party.push_back("Aerith");
	party.push_back("Al");
}

void Cursor::MoveUp()
{
	if (!isActive)
	{
		return;
	}
	if (contexts.at(currentContext).neighborList.at(currentPosition).north != "none")
	{
		currentPosition = contexts.at(currentContext).neighborList.at(currentPosition).north;
		screenPosition = contexts.at(currentContext).positions.at(currentPosition);
	}
}

void Cursor::MoveDown()
{
	if (!isActive)
	{
		return;
	}
	if (contexts.at(currentContext).neighborList.at(currentPosition).south != "none")
	{
		currentPosition = contexts.at(currentContext).neighborList.at(currentPosition).south;
		screenPosition = contexts.at(currentContext).positions.at(currentPosition);
	}
}

void Cursor::MoveRight()
{
	if (!isActive)
	{
		return;
	}
	if (contexts.at(currentContext).neighborList.at(currentPosition).east != "none")
	{
		currentPosition = contexts.at(currentContext).neighborList.at(currentPosition).east;
		screenPosition = contexts.at(currentContext).positions.at(currentPosition);
	}
}

void Cursor::MoveLeft()
{
	if (!isActive)
	{
		return;
	}
	if (contexts.at(currentContext).neighborList.at(currentPosition).west != "none")
	{
		currentPosition = contexts.at(currentContext).neighborList.at(currentPosition).west;
		screenPosition = contexts.at(currentContext).positions.at(currentPosition);
	}
}

void Cursor::ToggleActive(bool state)
{
	isActive = state;
	if (!isActive)
	{
		offset = invisibleOffset;
	}
	else
	{
		offset = visibleOffset;
	}
}

void Cursor::ContextSwitch(std::string newContext)
{
	ToggleActive(false);
	if (contexts.at(newContext).isOwned)
	{
		currentContext = newContext;
		currentPosition = ownedContexts.at(currentContext).startingLocation;
		ToggleActive(true);
		screenPosition = ownedContexts.at(currentContext).positions.at(currentPosition);
	}
	else
	{
		currentContext = newContext;
		currentPosition = contexts.at(currentContext).startingLocation;
		ToggleActive(true);
		screenPosition = contexts.at(currentContext).positions.at(currentPosition);
	}
}

void Cursor::Select()
{
	if (!isActive)
	{
		return;
	}
	if (contexts.at(currentContext).isFinalContext)
	{
		battle.AddAction(currentPosition);
		battle.GenerateAction();
		ContextSwitch("Main");
	}
	else
	{
		battle.AddAction(currentPosition);
		std::string resultantContext = contexts.at(currentContext).neighborList.at(currentPosition).resultantContext;
		if (contexts.at(resultantContext).isOwned)
		{
			ContextSwitch(party.at(battle.turnCount));
		}
		else
		{
			ContextSwitch(resultantContext);
		}
	}
}

std::string& Cursor::GetContext()
{
	return currentContext;
}

bool Cursor::CheckIfSubMenu()
{
	return contexts.at(currentContext).isSubMenu;
}
