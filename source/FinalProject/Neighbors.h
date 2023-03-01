#pragma once
#include <string>
#include <json/json.h>
struct Neighbors
{
	std::string north{"none"};
	std::string south{ "none" };
	std::string east{ "none" };
	std::string west{ "none" };
	std::string resultantContext{ "none" };

	Neighbors(Json::Value&);
};
