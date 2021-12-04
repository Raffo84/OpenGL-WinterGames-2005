#include ".\protready.h"

ProtReady::ProtReady(void)
{
	prot = PROT_READY_ID;
}

ProtReady::~ProtReady(void)
{
}

bool ProtReady::getReady()
{
	return ready;
}

void ProtReady::giveInformation(unsigned int newPlayerNum, bool newReady)
{
	playerNum = newPlayerNum;
	ready = newReady;
	createString();
}

void ProtReady::restString()
{
	unsigned int counter = counterBegin();

	// night
    memcpy(packetString+counter, &ready, sizeof(bool));
    counter += sizeof(bool);
}

void ProtReady::restInformation()
{
	unsigned int counter = counterBegin();

	// night
	memcpy(&ready, packetString+counter, sizeof(bool));
	counter += sizeof(bool);
}