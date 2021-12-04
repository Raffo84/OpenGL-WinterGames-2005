#pragma once
#include "networkpacket.h"

#include <iostream>
#include <cstring>
#include "../WinterGames/define.h"

class ProtError :
	public NetworkPacket
{
public:
	ProtError(void);
	~ProtError(void);

	void giveInformation(unsigned int newPlayerNum, unsigned int newError);

	unsigned int getError();

protected:
	unsigned int error;

	void restString();
	void restInformation();
};
