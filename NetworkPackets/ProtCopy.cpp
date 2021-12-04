#include ".\protcopy.h"

ProtCopy::ProtCopy(NetworkPacket *packet)
{
	prot = packet->getProt();
	playerNum = packet->getPlayerNum();

	unsigned int length = helper->getLength(prot);
    packetString = new char[length + sizeof(char)];
	char *copyName = packet->getString();
	for (unsigned int i = 0; i < length+1; i++) {
		packetString[i] = copyName[i];
	}
}

ProtCopy::ProtCopy(char *newPacketString)
{
	prot = newPacketString[0];
    unsigned int length = helper->getLength(prot);
    packetString = new char[length + sizeof(char)];
	for (unsigned int i = 0; i < length+1; i++) {
		packetString[i] = newPacketString[i];
	}
}

ProtCopy::~ProtCopy(void)
{
}

void ProtCopy::restString()
{
}

void ProtCopy::restInformation()
{
}