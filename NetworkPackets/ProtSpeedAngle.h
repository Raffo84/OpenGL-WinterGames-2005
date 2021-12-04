#pragma once
#include "networkpacket.h"

#include <iostream>
#include <cstring>
#include "../WinterGames/define.h"

class ProtSpeedAngle :
	public NetworkPacket
{
public:
	ProtSpeedAngle(void);
	~ProtSpeedAngle(void);

	void giveInformation(unsigned int playerNum, float newSpeed, float newAngle);

	float		getSpeed();
	float		getAngle();

protected:
	float		speed;
	float		angle;

	void restString();
	void restInformation();
};
