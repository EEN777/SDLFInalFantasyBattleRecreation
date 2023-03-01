#include "Enemy.h"
#include "RectTool.h"
#include <iostream>

using namespace RectTool;

Enemy::Enemy(SDL_Texture* spriteSheet, Json::Value& data) :
	Character{ spriteSheet,GenerateRect(data["Offset"]["RectData"]), GenerateRect(data["ScreenPosition"]["RectData"]),data["Name"].asString() },
	_maxHealth{ data["HP"].asInt() }, _health{ _maxHealth }, stats{}
{
	size_t iterator{ 0 };
	for (auto& stat : data["Stats"])
	{
		stats[iterator] = stat.asInt();
		++iterator;
	}
}

void Enemy::TakeDamage(int damage)
{
	_health -= damage;
	if (_health < 0)
	{
		_health = 0;
		isDead = true;
	}
	std::cout << "Chaos took " << damage << " points of damage! HP: " << _health << std::endl;
}

int Enemy::GetHealth()
{
	return _health;
}

int Enemy::GetStat(Stats stat)
{
	return stats[stat];
}
