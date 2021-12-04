#pragma once
#include "networkpacket.h"

#include <iostream>
#include <cstring>
#include "../WinterGames/define.h"

class ProtLobby :
	public NetworkPacket
{
public:
	ProtLobby(void);
	~ProtLobby(void);

	void giveInformation(unsigned int newPlayerNum);

protected:
	void restString();
	void restInformation();
};
