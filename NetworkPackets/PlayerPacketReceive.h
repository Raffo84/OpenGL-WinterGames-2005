#pragma once

#include <iostream>
#include <vector>
#include "../WinterGames/define.h"

#include "PlayerPacketHelper.h"

class PlayerPacketReceive
{
public:
	PlayerPacketReceive(void);
	~PlayerPacketReceive(void);

	void addBuffer(char *buffer, unsigned long length);
	char* getPacket();
	bool packetsReady();

protected:
	std::vector<char> incomingBytes;
	std::vector<char>::iterator incomingBytesIT;
	std::vector<char> currentPacket;
	std::vector<char*> completedPackets;
	PlayerPacketHelper *helper;
	void checkForPackets();
	void checkPacketFinish();
};
