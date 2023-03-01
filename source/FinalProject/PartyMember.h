#pragma once
#include <json/json.h>
#include <unordered_map>
#include "Character.h"
class PartyMember : public Character
{
private:
	int _maxHealth;
	int _health;
	int stats[7];
	int frameCount{ 0 };
	const std::string job;
	std::unordered_map<std::string, SDL_Rect> animations{};
public:
	SDL_Rect healthPosition{};
	bool isDead{ false };
	enum Stats
	{
		Strength,
		Agility,
		Int,
		Luck,
		Attack,
		Accuracy,
		Evasion
	};

	enum Actions
	{
		Default,
		Walk,
		WalkBack,
		Victory
	};

	Actions action{Default};
	PartyMember(SDL_Texture* spriteSheet,Json::Value& data);

	void ChangeSprite(std::string);

	void EventTick();
	void WalkForward();
	void WalkBackwards();
	void VictoryDance();
	int GetStat(Stats);
	void PerformCommand();
	void TakeDamage(int);
	void GetHealed(int);
	int GetHealth();

};

