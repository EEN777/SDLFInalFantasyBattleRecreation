#include "Context.h"
#include "RectTool.h"

using namespace RectTool;

Context::Context(Json::Value& data) :
	startingLocation{ data["StartingLocation"].asString() }, isFinalContext{ data["isFinalContext"].asBool() }, isSubMenu{ data["isSubMenu"] ? data["isSubMenu"].asBool() : false},
	isOwned{ data["isOwned"] ? data["isOwned"].asBool() : false }
{
	for (auto& position : data["Positions"])
	{
		positions.emplace(position["Name"].asString(), GenerateRect(position["RectData"]));
		neighborList.emplace(position["Name"].asString(), position);
	}
}