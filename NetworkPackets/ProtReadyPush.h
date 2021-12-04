#pragma once
#include "networkpacket.h"

#include <iostream>
#include <cstring>
#include "../WinterGames/define.h"

class ProtReadyPush :
	public NetworkPacket
{
public:
	ProtReadyPush(void);
	~ProtReadyPush(void);

	void giveInformation(unsigned int newPlayerNum);

protected:
	void restString();
	void restInformation();
};
