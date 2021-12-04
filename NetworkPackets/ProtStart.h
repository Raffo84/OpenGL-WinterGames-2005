#pragma once
#include "networkpacket.h"

class ProtStart :
	public NetworkPacket
{
public:
	ProtStart(void);
	~ProtStart(void);

	void giveInformation(unsigned int playerNum, unsigned int newObsSeed, float newPlayerStartAngle);

	unsigned int getObsSeed();
	float getPlayerStartAngle();

protected:
	unsigned int obsSeed;
	float playerStartAngle;

	void restString();
	void restInformation();
};
