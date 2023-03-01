#pragma once
#include "TurnHandler.h"
#include "Game.h"

class SequencePlayer
{
	Game& game;
	TurnHandler& turnHandler;

	int currentFrame{};
	int sequenceNumber{};
	int activeTarget{};

	bool hasStartedPlaying{ false };
	bool sequenceOver{ false };
	bool chaosUsingThundaga{ false };
	bool chaosUsingBlizzaga{ false };

public:
	bool isGameComplete{ false };
	bool partyLost{ false };
	SequencePlayer(Game&, TurnHandler&);
	void PlaySequence();
	void HandleAction();
	void HandleEnemy();
	void CheckAttack();
	void CheckHeal();
	void CheckMagic();
};

