#pragma once
#include "player.h"
#include "../WinterGames/define.h"

class HumanPlayer :
	public Player
{
public:
	HumanPlayer(void);
	~HumanPlayer(void);

	int getPlayerType() const { return PLAYER_TYPE_HUMAN; }
};
