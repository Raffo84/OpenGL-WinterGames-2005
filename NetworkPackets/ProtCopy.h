#pragma once
#include "networkpacket.h"
#include "../WinterGames/define.h"

class ProtCopy :
	public NetworkPacket
{
public:
	ProtCopy(NetworkPacket *packet);
	ProtCopy(char *packetString);
	~ProtCopy(void);

protected:
	void restString();
	void restInformation();
};
