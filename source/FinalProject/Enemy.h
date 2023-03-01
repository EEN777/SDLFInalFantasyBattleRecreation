#pragma once
#include <json/json.h>
#include "Character.h"

class Enemy : public Character
{
private:
	int _maxHealth;
	int _health;
	int stats[5];
public:
	bool isDead{ false };

	Enemy(SDL_Texture* spriteSheet, Json::Value& data);
	enum Stats
	{
		Attack,
		Defense,
		MagicDefense,
		Evasion,
		Accuracy
	};
	void TakeDamage(int damage);
	int GetHealth();
	int GetStat(Stats);

};

