#include "Action.h"

Action::Action(std::string action1, std::string action2) :
	first{ action1 }, second{ action2 }, third{"none"}
{
}

Action::Action(std::string action1, std::string action2, std::string action3):
	first{ action1 }, second{ action2 }, third{ action3 }
{
}
