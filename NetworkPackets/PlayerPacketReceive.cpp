#include ".\playerPacketReceive.h"

PlayerPacketReceive::PlayerPacketReceive(void)
{
	helper = new PlayerPacketHelper();
}

PlayerPacketReceive::~PlayerPacketReceive(void)
{
	delete helper;
}

void PlayerPacketReceive::addBuffer(char *buffer, unsigned long length)
{
	for (unsigned long i = 0; i < length; i++) {
        incomingBytes.push_back(buffer[i]);
	}
	checkForPackets();
}

char* PlayerPacketReceive::getPacket()
{
	// return the first packet in completedPackets
	char* returnChar = completedPackets[0];
	for (unsigned int i = 0; i < completedPackets.size() - 1; i++) {
		completedPackets[i] = completedPackets[i + 1];
	}
	completedPackets.pop_back();
	return returnChar;
}

bool PlayerPacketReceive::packetsReady()
{
	if (completedPackets.size() > 0) return true;
	return false;
}

void PlayerPacketReceive::checkForPackets()
{
	//init iterator
	incomingBytesIT = incomingBytes.begin();

	// copying from incoming bytes to the current packet till incoming bytes are empty
	while (incomingBytesIT != incomingBytes.end()) {		
		currentPacket.push_back(incomingBytes[0]);
		incomingBytesIT = incomingBytes.erase(incomingBytesIT);
		checkPacketFinish();
	}
}

void PlayerPacketReceive::checkPacketFinish()
{
	// getting length of current packet
	unsigned int length = helper->getLength(currentPacket[0]);
	// if currentPacket.size() = length, then packet is complete
	if (currentPacket.size() >= length + 1) {
		char* complete = new char[length + sizeof(char)];
		for (unsigned int i = 0; i < length + sizeof(char); i++) {
			complete[i] = currentPacket[i];
		}
		currentPacket.clear();
		completedPackets.push_back(complete);
	}
}