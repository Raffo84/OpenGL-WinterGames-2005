#include ".\networkpacket.h"

NetworkPacket::NetworkPacket(void)
{
	helper = new PlayerPacketHelper();
}

NetworkPacket::~NetworkPacket(void)
{
}

void NetworkPacket::giveString(char *newPacketString)
{
	packetString = newPacketString;
	createInformation();
}

char* NetworkPacket::getString()
{
	return packetString;
}

unsigned int NetworkPacket::getPlayerNum()
{
	return playerNum;
}

char NetworkPacket::getProt()
{
	return prot;
}

void NetworkPacket::createString()
{
	packetString = new char[helper->getLength(prot) + sizeof(char)];
	memcpy(packetString, &prot, sizeof(char));
	memcpy(packetString+sizeof(char), &playerNum, sizeof(unsigned int));
	restString();
}

void NetworkPacket::createInformation()
{
	// prot type
	if (packetString[0] != prot) return;

	// getting playernum
	memcpy(&playerNum, packetString+sizeof(char), sizeof(unsigned int));

	restInformation();
}

unsigned int NetworkPacket::counterBegin()
{
	return (sizeof(char) + sizeof(unsigned int));
}