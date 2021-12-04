#pragma once
#include "player.h"
#include "../WinterGames/define.h"

class LANPlayer :
	public Player
{
public:
	LANPlayer(void);
	~LANPlayer(void);

	int getPlayerType() const { return PLAYER_TYPE_LAN; }
	void setNameColor(Color* newCol) { nameCol = newCol; }
	void render() const;

private:
	Color*	nameCol;
};
