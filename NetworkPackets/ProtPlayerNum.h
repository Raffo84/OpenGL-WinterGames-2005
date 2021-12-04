#pragma once
#include "networkpacket.h"

#include <iostream>
#include <cstring>
#include "../WinterGames/define.h"

class ProtPlayerNum :
	public NetworkPacket
{
public:
	ProtPlayerNum(void);
	~ProtPlayerNum(void);

	void giveInformation(unsigned int playerNum);

protected:
	void restString();
	void restInformation();
};
