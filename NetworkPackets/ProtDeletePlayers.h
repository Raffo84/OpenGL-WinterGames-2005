#pragma once
#include "networkpacket.h"

#include <iostream>
#include <cstring>
#include "../WinterGames/define.h"

class ProtDeletePlayers :
	public NetworkPacket
{
public:
	ProtDeletePlayers(void);
	~ProtDeletePlayers(void);

	void giveInformation(unsigned int playerNum, unsigned int newDeletePlayers);
	unsigned int getDeletePlayers();

protected:
	unsigned int deletePlayers;

	void restString();
	void restInformation();
};
