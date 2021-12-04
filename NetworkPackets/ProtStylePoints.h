#pragma once
#include "networkpacket.h"

#include <iostream>
#include <cstring>
#include "../WinterGames/define.h"

class ProtStylePoints :
	public NetworkPacket
{
public:
	ProtStylePoints(void);
	~ProtStylePoints(void);

	void giveInformation(unsigned int playerNum, long newStylePoints);

	long getStylePoints();

protected:
	long stylePoints;

	void restString();
	void restInformation();
};
