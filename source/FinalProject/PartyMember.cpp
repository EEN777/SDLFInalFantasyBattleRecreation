#include "PartyMember.h"
#include "RectTool.h"
#include <iostream>

using namespace RectTool;

PartyMember::PartyMember(SDL_Texture* spriteSheet,Json::Value& data):
	Character{ spriteSheet,GenerateRect(data["Offset"]["RectData"]), GenerateRect(data["ScreenPosition"]["RectData"]),data["Name"].asString()},
	_maxHealth{ data["HP"].asInt() }, _health{ _maxHealth }, stats{}, job{data["Class"].asString()}
{
	healthPosition = GenerateRect(data["HealthPosition"]["RectData"]);
	for (auto& animation : data["Animations"])
	{
		animations.emplace(animation["Name"].asString(), GenerateRect(animation["RectData"]));
	}

	size_t iterator{ 0 };
	for (auto& stat : data["Stats"])
	{
		stats[iterator] = stat.asInt();
		++iterator;
	}
}

void PartyMember::ChangeSprite(std::string animationName)
{
	offset = animations.at(animationName);
}

void PartyMember::EventTick()
{
	switch (action)
	{
	case Default:
		break;
	case Walk:
		WalkForward();
		break;
	case WalkBack:
		WalkBackwards();
		break;
	case Victory:
		VictoryDance();
		break;
	default:
		break;
	}
}

void PartyMember::WalkForward()
{
	if (frameCount >= 16)
	{
		frameCount = 0;
		action = Default;

		if (_health < (_maxHealth / 4))
		{
			ChangeSprite("LowHealth");
		}
		return;
	}
	if (!isDead)
	{
		if (((frameCount / 4) % 2) == 0)
		{
			ChangeSprite("Walk");
		}
		else
		{
			ChangeSprite("Idle");
		}
		++frameCount;
		if (screenPosition.x > 576)
		{
			screenPosition.x -= 4;
		}
	}
}

void PartyMember::WalkBackwards()
{
	if (frameCount >= 16)
	{
		frameCount = 0;
		action = Default;

		if (_health < (_maxHealth / 4))
		{
			ChangeSprite("LowHealth");
		}
		return;
	}
	if (!isDead)
	{
		if (((frameCount / 4) % 2) == 0)
		{
			ChangeSprite("Walk");
		}
		else
		{
			ChangeSprite("Idle");
		}
		++frameCount;
		if (screenPosition.x < 640)
		{
			screenPosition.x += 4;
		}

	}
}

void PartyMember::VictoryDance()
{
	if (frameCount == 0)
	{
		ChangeSprite("Casting");
	}

	if (frameCount == 20)
	{
		ChangeSprite("Idle");
	}

	if (frameCount == 40)
	{
		frameCount = 0;
		return;
	}
	++frameCount;
}

void PartyMember::PerformCommand()
{
	ChangeSprite("Casting");
}

void PartyMember::TakeDamage(int damage)
{
	_health -= damage;
	if (_health < (_maxHealth / 4) && !isDead)
	{
		ChangeSprite("LowHealth");
	}
	if (_health <= 0)
	{
		_health = 0;
		ChangeSprite("KO");
		isDead = true;
	}

	std::cout << GetName() <<  " took " << damage << " points of damage! HP: " << _health << std::endl;
}

void PartyMember::GetHealed(int healAmount)
{
	_health += healAmount;
	if (_health > _maxHealth)
	{
		_health = _maxHealth;
	}
	std::cout << GetName() << " healed for " << healAmount << " points! HP: " << _health << std::endl;

}

int PartyMember::GetHealth()
{
	return _health;
}

int PartyMember::GetStat(Stats stat)
{
	return stats[stat];
}
