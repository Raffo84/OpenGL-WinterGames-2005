#pragma once
#include "networkpacket.h"

#include <iostream>
#include <cstring>
#include "../WinterGames/define.h"

class ProtVersion :
	public NetworkPacket
{
public:
	ProtVersion(void);
	~ProtVersion(void);

	void giveInformation(unsigned int newPlayerNum, unsigned int newVersion);

	unsigned int getVersion();

protected:
	unsigned int version;

	void restString();
	void restInformation();
};
