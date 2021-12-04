#pragma once
#include "networkpacket.h"

#include <iostream>
#include <cstring>
#include "../WinterGames/define.h"

class ProtReady :
	public NetworkPacket
{
public:
	ProtReady(void);
	~ProtReady(void);

	void giveInformation(unsigned int newPlayerNum, bool newReady);

	bool getReady();

protected:
	bool ready;

	void restString();
	void restInformation();
};
