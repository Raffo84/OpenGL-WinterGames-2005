#pragma once
#include "networkpacket.h"

#include <iostream>
#include <cstring>
#include "../WinterGames/define.h"

class ProtGameType :
	public NetworkPacket
{
public:
	ProtGameType(void);
	~ProtGameType(void);

	void giveInformation(unsigned int newPlayerNum, unsigned int newGameType);

	unsigned int getGameType();

protected:
	unsigned int gameType;

	void restString();
	void restInformation();
};
