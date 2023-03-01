#include "Neighbors.h"

Neighbors::Neighbors(Json::Value& data)
{
	if (data["North"])
	{
		north = data["North"].asString();
	}

	if (data["South"])
	{
		south = data["South"].asString();
	}

	if (data["East"])
	{
		east = data["East"].asString();
	}

	if (data["West"])
	{
		west = data["West"].asString();
	}

	if (data["ResultantContext"])
	{
		resultantContext = data["ResultantContext"].asString();
	}
}