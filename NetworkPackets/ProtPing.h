#pragma once
#include "networkpacket.h"

#include <iostream>
#include <cstring>
#include "../WinterGames/define.h"

class ProtPing :
	public NetworkPacket
{
public:
	ProtPing(void);
	~ProtPing(void);

	void giveInformation(unsigned int newPlayerNum);

protected:
	void restString();
	void restInformation();
};
