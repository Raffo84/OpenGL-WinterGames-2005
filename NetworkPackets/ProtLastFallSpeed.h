#pragma once
#include "networkpacket.h"

#include <iostream>
#include <cstring>
#include "../WinterGames/define.h"

class ProtLastFallSpeed :
	public NetworkPacket
{
public:
	ProtLastFallSpeed(void);
	~ProtLastFallSpeed(void);

	void giveInformation(unsigned int playerNum, float newLastFallSpeed);

	float getLastFallSpeed();

protected:
	float lastFallSpeed;

	void restString();
	void restInformation();
};
