#pragma once
#include "PlayerInput.h"

class KeyboardInput : public PlayerInput
{
public:
	PlayerActions GetPlayerInput() override;
};

