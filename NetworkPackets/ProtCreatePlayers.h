#pragma once
#include "networkpacket.h"

#include <iostream>
#include <cstring>
#include "../WinterGames/define.h"

class ProtCreatePlayers :
	public NetworkPacket
{
public:
	ProtCreatePlayers(void);
	~ProtCreatePlayers(void);

	void giveInformation(unsigned int playerNum, unsigned int newCreatePlayers);
	unsigned int getCreatePlayers();

protected:
	unsigned int createPlayers;

	void restString();
	void restInformation();
};
