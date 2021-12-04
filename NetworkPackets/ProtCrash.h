#pragma once
#include "networkpacket.h"

#include <iostream>
#include <cstring>
#include "../WinterGames/define.h"

class ProtCrash :
	public NetworkPacket
{
public:
	ProtCrash(void);
	~ProtCrash(void);

	void giveInformation(unsigned int playerNum, unsigned int newCrashedAgainst, char newCrashType);

	unsigned int getCrashedAgainst();
	char getCrashType();

protected:
	unsigned int crashedAgainst;
	char crashType;

	void restString();
	void restInformation();
};
