#pragma once

#include <iostream>
#include <cstring>
#include "../WinterGames/define.h"

#include "PlayerPacketHelper.h"

class NetworkPacket
{
public:
	NetworkPacket(void);
	~NetworkPacket(void);

	void giveString(char *newPacket);
	char* getString();

	unsigned int getPlayerNum();
	char getProt();

protected:
	char *packetString;
	unsigned int playerNum;
	char prot;
	PlayerPacketHelper *helper;

	void createString();
    virtual void restString() = 0;
	void createInformation();
	virtual void restInformation() = 0;

	unsigned int counterBegin();
};
