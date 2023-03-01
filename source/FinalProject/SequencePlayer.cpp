#include "SequencePlayer.h"
#include "Random.h"

SequencePlayer::SequencePlayer(Game& gameInstance, TurnHandler& handlerInstance) :
	game{ gameInstance }, turnHandler{handlerInstance}
{
}

void SequencePlayer::PlaySequence()
{
	if (!hasStartedPlaying)
	{
		turnHandler.SetStatus(false);
		hasStartedPlaying = true;
	}
	else
	{
		game.menus.at("BattleCanvas").ToggleActive(true);
		switch (sequenceNumber)
		{
		case 0:
			HandleAction();
			break;
		case 1:
			HandleAction();
			break;
		case 2:
			HandleAction();
			break;
		case 3:
			HandleAction();
			break;
		case 4:
			if (game.enemies.at("Chaos").isDead)
			{
				++sequenceNumber;
				break;
			}
			else
			{
				HandleEnemy();
			}
			break;
		case 5:
			turnHandler.SetStatus(true);
			game.cursors.at(0).ContextSwitch("Main");
			game.battle.actionQueue.clear();
			game.battle.turnCount = 0;
			turnHandler.Clear();
			hasStartedPlaying = false;
			sequenceNumber = 0;
			game.menus.at("BattleCanvas").ToggleActive(false);
			if (game.enemies.at("Chaos").isDead)
			{
				isGameComplete = true;
			}
			else
			{
				int deadCount{};
				for (auto& member : game.partyMembers)
				{
					if (member.second.isDead)
					{
						++deadCount;
					}
				}
				if (deadCount >= 4)
				{
					isGameComplete = true;
					partyLost = true;
				}
				deadCount;
			}
			break;
		}
	}

}

void SequencePlayer::HandleAction()
{
	if (currentFrame == 0)
	{
		if (game.partyMembers.at(turnHandler.GetPartyMember(sequenceNumber)).isDead)
		{
			if ((sequenceNumber + 1) <= 3)
			{
				sequenceNumber++;
				game.partyMembers.at(turnHandler.GetPartyMember(sequenceNumber)).action = PartyMember::Walk;
			}
			else
			{
				sequenceNumber++;
				return;
			}
		}
		if (sequenceNumber == 0)
		{
			game.partyMembers.at(turnHandler.GetPartyMember(sequenceNumber)).action = PartyMember::Walk;
			if (game.battle.actionQueue.at(sequenceNumber).third == "none")
			{
				game.subjectPanels.at(game.battle.actionQueue.at(sequenceNumber).second).ToggleActive(true);
			}
			else
			{
				if (game.battle.actionQueue.at(sequenceNumber).first == "Magic")
				{
					game.actionPanels.at(game.battle.actionQueue.at(sequenceNumber).second).ToggleActive(true);
				}
			}
		}
		game.actorPanels.at(turnHandler.GetPartyMember(sequenceNumber)).ToggleActive(true);
		if (game.battle.actionQueue.at(sequenceNumber).third == "none")
		{
			game.subjectPanels.at(game.battle.actionQueue.at(sequenceNumber).second).ToggleActive(true);
		}
		else
		{
			if(game.battle.actionQueue.at(sequenceNumber).first == "Magic")
			{
				game.actionPanels.at(game.battle.actionQueue.at(sequenceNumber).second).ToggleActive(true);
			}
		}
	}

	if (currentFrame == 20)
	{
		if (!game.partyMembers.at(turnHandler.GetPartyMember(sequenceNumber)).isDead)
		{
			game.partyMembers.at(turnHandler.GetPartyMember(sequenceNumber)).ChangeSprite("Casting");
			if (game.battle.actionQueue.at(sequenceNumber).third == "none")
			{
				game.partyWeapons.at(turnHandler.GetPartyMember(sequenceNumber)).ToggleActive(true);
			}
			else
			{
				if (game.battle.actionQueue.at(sequenceNumber).second != "Blizzaga" && game.battle.actionQueue.at(sequenceNumber).second != "Thundaga")
				{
					Mix_PlayChannel(-1, game.restoreSfx, 0);
				}
				else
				{
					Mix_PlayChannel(-1, game.blackMagicSfx, 0);
				}
			}

		}
	}

	if (currentFrame > 20 && currentFrame < 40)
	{
		if (game.battle.actionQueue.at(sequenceNumber).first == "Magic")
		{
			if ((currentFrame % 5) == 0)
			{
				SDL_SetRenderDrawColor(game.renderer, 0x0, 0x0, 0x0, 0x0);
			}
			else
			{
				if (game.battle.actionQueue.at(sequenceNumber).second == "Thundaga")
				{
					SDL_SetRenderDrawColor(game.renderer, 0xFF, 0xBD, 0x00, 0x0);
				}
				else if (game.battle.actionQueue.at(sequenceNumber).second == "Blizzaga")
				{
					SDL_SetRenderDrawColor(game.renderer, 0x6B, 0x8C, 0xFF, 0x0);
				}
				else
				{
					SDL_SetRenderDrawColor(game.renderer, 0xBD, 0xFF, 0x18, 0x0);
				}
			}
		}
	}

	if (currentFrame == 40)
	{
		if (!game.partyMembers.at(turnHandler.GetPartyMember(sequenceNumber)).isDead)
		{
			SDL_SetRenderDrawColor(game.renderer, 0x0, 0x0, 0x0, 0x0);
			if (game.battle.actionQueue.at(sequenceNumber).third == "none")
			{
				CheckAttack();
			}
			else
			{
				if (game.battle.actionQueue.at(sequenceNumber).third != "Dead")
				{
					game.subjectPanels.at(game.battle.actionQueue.at(sequenceNumber).third).ToggleActive(true);
				}
				game.resultPanels.at("Result").ToggleActive(true);
				if (game.battle.actionQueue.at(sequenceNumber).third == "Chaos")
				{
					CheckMagic();
				}
				else
				{
					CheckHeal();
				}
			}
		}
	}
	if (currentFrame >= 120)
	{
		game.partyMembers.at(turnHandler.GetPartyMember(sequenceNumber)).action = PartyMember::WalkBack;
		if ((sequenceNumber + 1) <= 3)
		{
			game.partyMembers.at(turnHandler.GetPartyMember(sequenceNumber + 1)).action = PartyMember::Walk;
		}
		game.actorPanels.at(turnHandler.GetPartyMember(sequenceNumber)).ToggleActive(false);
		if (game.battle.actionQueue.at(sequenceNumber).third == "none")
		{
			game.subjectPanels.at(game.battle.actionQueue.at(sequenceNumber).second).ToggleActive(false);
			game.actionPanels.at("Hit").ToggleActive(false);
			game.actionPanels.at("Miss").ToggleActive(false);
			game.resultPanels.at("Result").ToggleActive(false);
			game.displayingResult = false;
			game.partyWeapons.at(turnHandler.GetPartyMember(sequenceNumber)).ToggleActive(false);
		}
		else
		{
			if (game.battle.actionQueue.at(sequenceNumber).first == "Magic")
			{
				game.actionPanels.at(game.battle.actionQueue.at(sequenceNumber).second).ToggleActive(false);
				game.subjectPanels.at(game.battle.actionQueue.at(sequenceNumber).third).ToggleActive(false);
				game.resultPanels.at("Result").ToggleActive(false);
				game.displayingResult = false;
			}
		}
		currentFrame = 0;
		++sequenceNumber;
	}
	else
	{
		++currentFrame;
	}

}

void SequencePlayer::CheckAttack()
{
	auto hitChance = 168 + game.partyMembers.at(turnHandler.GetPartyMember(sequenceNumber)).GetStat(PartyMember::Accuracy);
	if (hitChance > 255)
	{
		hitChance = 255;
	}
	hitChance -= game.enemies.at("Chaos").GetStat(Enemy::Evasion);

	if (Roll(0, 200) <= hitChance)
	{
		game.actionPanels.at("Hit").ToggleActive(true);
		game.resultPanels.at("Result").ToggleActive(true);
		game.enemies.at("Chaos").TakeDamage(game.partyMembers.at(turnHandler.GetPartyMember(sequenceNumber)).GetStat(PartyMember::Attack));
		Mix_PlayChannel(-1, game.hitSfx, 0);
		game.CreateResult(game.partyMembers.at(turnHandler.GetPartyMember(sequenceNumber)).GetStat(PartyMember::Attack));
		game.displayingResult = true;
	}
	else
	{
		game.actionPanels.at("Miss").ToggleActive(true);
		std::cout << turnHandler.GetPartyMember(sequenceNumber) << " missed!" << std::endl;
		Mix_PlayChannel(-1, game.missSfx, 0);
	}
}

void SequencePlayer::CheckMagic()
{
	auto hitChance = 168 + game.partyMembers.at(turnHandler.GetPartyMember(sequenceNumber)).GetStat(PartyMember::Accuracy);
	if (hitChance > 255)
	{
		hitChance = 255;
	}
	hitChance -= game.enemies.at("Chaos").GetStat(Enemy::Evasion);

	if (Roll(0, 200) <= hitChance)
	{
		auto minDamage = 2 * game.spellStrength.at(game.battle.actionQueue.at(sequenceNumber).second);
		auto maxDamage = 4 * game.spellStrength.at(game.battle.actionQueue.at(sequenceNumber).second);
		auto result = Roll(minDamage, maxDamage);
		game.CreateResult(result);
		game.displayingResult = true;
		std::cout << turnHandler.GetPartyMember(sequenceNumber) << " cast " << game.battle.actionQueue.at(sequenceNumber).second << " on Chaos!" << std::endl;
		game.enemies.at("Chaos").TakeDamage(result);
	}
	else
	{
		auto minDamage = game.spellStrength.at(game.battle.actionQueue.at(sequenceNumber).second);
		auto maxDamage = 2 * game.spellStrength.at(game.battle.actionQueue.at(sequenceNumber).second);
		auto result = Roll(minDamage, maxDamage);
		game.CreateResult(result);
		game.displayingResult = true;
		std::cout << turnHandler.GetPartyMember(sequenceNumber) << " cast " << game.battle.actionQueue.at(sequenceNumber).second << " on Chaos!" << std::endl;
		game.enemies.at("Chaos").TakeDamage(result);
	}
}

void SequencePlayer::CheckHeal()
{
	if (game.battle.actionQueue.at(sequenceNumber).second == "Cure" || game.battle.actionQueue.at(sequenceNumber).second == "Curaga")
	{
		auto minAmount = game.spellStrength.at(game.battle.actionQueue.at(sequenceNumber).second);
		auto maxAmount = 2 * game.spellStrength.at(game.battle.actionQueue.at(sequenceNumber).second);
		auto result = Roll(minAmount, maxAmount);
		game.CreateResult(result);
		game.displayingResult = true;
		game.partyMembers.at(game.battle.actionQueue.at(sequenceNumber).third).GetHealed(result);
	}
	else if(game.battle.actionQueue.at(sequenceNumber).second == "Life")
	{
		game.partyMembers.at(game.battle.actionQueue.at(sequenceNumber).third).isDead = false;
		game.partyMembers.at(game.battle.actionQueue.at(sequenceNumber).third).GetHealed(1);
		game.partyMembers.at(game.battle.actionQueue.at(sequenceNumber).third).ChangeSprite("LowHealth");
	}

}

void SequencePlayer::HandleEnemy()
{
	++currentFrame;
	if (currentFrame == 1)
	{
		game.actorPanels.at("Chaos").ToggleActive(true);
		auto target = Roll(0, 3);
		activeTarget = target;
		auto randomCheck = Roll(0, 100);
		if (randomCheck < 50)
		{
			game.subjectPanels.at(turnHandler.GetPartyMember(target)).ToggleActive(true);
			auto hitChance = 168 + game.enemies.at("Chaos").GetStat(Enemy::Accuracy);
			if (hitChance > 255)
			{
				hitChance = 255;
			}
			hitChance -= game.partyMembers.at(turnHandler.GetPartyMember(target)).GetStat(PartyMember::Evasion);
			if (Roll(0, 200) <= hitChance)
			{
				game.actionPanels.at("Hit").ToggleActive(true);
				game.resultPanels.at("Result").ToggleActive(true);
				game.partyMembers.at(turnHandler.GetPartyMember(target)).TakeDamage(game.enemies.at("Chaos").GetStat(Enemy::Attack));
				game.CreateResult(game.enemies.at("Chaos").GetStat(Enemy::Attack));
				game.displayingResult = true;
				Mix_PlayChannel(-1, game.hitSfx, 0);
			}

			else
			{
				game.actionPanels.at("Miss").ToggleActive(true);
				std::cout << turnHandler.GetPartyMember(sequenceNumber) << " missed!" << std::endl;
				Mix_PlayChannel(-1, game.missSfx, 0);
			}

		}

		else
		{
			game.subjectPanels.at(turnHandler.GetPartyMember(target)).ToggleActive(true);
			auto magicRoll = Roll(0, 100);

			if (magicRoll > 50)
			{
				chaosUsingBlizzaga = true;
				game.actionPanels.at("Blizzaga").ToggleActive(true);
				game.resultPanels.at("Result").ToggleActive(true);
				game.partyMembers.at(turnHandler.GetPartyMember(target)).TakeDamage(250);
				game.CreateResult(250);
				game.displayingResult = true;
			}

			else
			{
				chaosUsingThundaga = true;
				game.actionPanels.at("Thundaga").ToggleActive(true);
				game.resultPanels.at("Result").ToggleActive(true);
				game.partyMembers.at(turnHandler.GetPartyMember(target)).TakeDamage(175);
				game.CreateResult(175);
				game.displayingResult = true;
			}
			Mix_PlayChannel(-1, game.blackMagicSfx, 0);
			std::cout << "Chaos uses Magic!" << std::endl;
		}
	}

	if (currentFrame > 1 && currentFrame < 22)
	{
		if (chaosUsingBlizzaga)
		{
			if ((currentFrame % 5) == 0)
			{
				SDL_SetRenderDrawColor(game.renderer, 0x0, 0x0, 0x0, 0x0);
			}
			else
			{
				SDL_SetRenderDrawColor(game.renderer, 0x6B, 0x8C, 0xFF, 0x0);
			}
		}
		else if (chaosUsingThundaga)
		{
			if ((currentFrame % 5) == 0)
			{
				SDL_SetRenderDrawColor(game.renderer, 0x0, 0x0, 0x0, 0x0);
			}
			else
			{
				SDL_SetRenderDrawColor(game.renderer, 0xFF, 0xBD, 0x00, 0x0);
			}
		}
	}

	if (currentFrame == 23)
	{
		SDL_SetRenderDrawColor(game.renderer, 0x0, 0x0, 0x0, 0x0);
	}

	if (currentFrame == 100)
	{
		chaosUsingThundaga = false;
		chaosUsingBlizzaga = false;
		game.actorPanels.at("Chaos").ToggleActive(false);
		game.subjectPanels.at(turnHandler.GetPartyMember(activeTarget)).ToggleActive(false);
		game.actionPanels.at("Hit").ToggleActive(false);
		game.resultPanels.at("Result").ToggleActive(false);
		game.displayingResult = false;
		game.actionPanels.at("Miss").ToggleActive(false);
		game.actionPanels.at("Blizzaga").ToggleActive(false);
		game.actionPanels.at("Thundaga").ToggleActive(false);
		currentFrame = 0;
		++sequenceNumber;
	}

}