#pragma once
#include "networkpacket.h"

#include <iostream>
#include <cstring>
#include "../WinterGames/define.h"

class ProtFinalTime :
	public NetworkPacket
{
public:
	ProtFinalTime(void);
	~ProtFinalTime(void);

	void giveInformation(unsigned int playerNum, double newFinalTime);

	double getFinalTime();

protected:
	double finalTime;

	void restString();
	void restInformation();
};
