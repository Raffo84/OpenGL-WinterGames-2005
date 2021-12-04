#include ".\protstart.h"

ProtStart::ProtStart(void)
{
	prot = PROT_START_ID;
}

ProtStart::~ProtStart(void)
{
}

void ProtStart::giveInformation(unsigned int newPlayerNum, unsigned int newObsSeed, float newPlayerStartAngle)
{
	playerNum = newPlayerNum;
	obsSeed = newObsSeed;
	createString();
}

unsigned int ProtStart::getObsSeed()
{
	return obsSeed;
}

float ProtStart::getPlayerStartAngle()
{
	return playerStartAngle;
}

void ProtStart::restString()
{
	unsigned int counter = counterBegin();

	// obsSeed
    memcpy(packetString+counter, &obsSeed, sizeof(unsigned int));
    counter += sizeof(unsigned int);
	// playerStartAngle
    memcpy(packetString+counter, &playerStartAngle, sizeof(float));
    counter += sizeof(float);
}

void ProtStart::restInformation()
{
	unsigned int counter = counterBegin();

	// obsSeed
	memcpy(&obsSeed, packetString+counter, sizeof(unsigned int));
	counter += sizeof(unsigned int);
	// playerStartAngle
	memcpy(&playerStartAngle, packetString+counter, sizeof(float));
	counter += sizeof(float);
}