#pragma once
#include "networkpacket.h"

#include <iostream>
#include <cstring>
#include "../WinterGames/define.h"

class ProtDayTime :
	public NetworkPacket
{
public:
	ProtDayTime(void);
	~ProtDayTime(void);

	void giveInformation(unsigned int newPlayerNum, unsigned int newNight);

	unsigned int getNight();

protected:
	unsigned int night;

	void restString();
	void restInformation();
};
