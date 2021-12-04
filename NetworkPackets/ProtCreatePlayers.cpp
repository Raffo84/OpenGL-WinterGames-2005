#include ".\protcreateplayers.h"

ProtCreatePlayers::ProtCreatePlayers(void)
{
	prot = PROT_CREATE_PLAYERS_ID;
}

ProtCreatePlayers::~ProtCreatePlayers(void)
{
}

void ProtCreatePlayers::giveInformation(unsigned int newPlayerNum, unsigned int newCreatePlayers)
{
	playerNum = newPlayerNum;
	createPlayers = newCreatePlayers;
	createString();
}

unsigned int ProtCreatePlayers::getCreatePlayers()
{
	return createPlayers;
}

void ProtCreatePlayers::restString()
{
	unsigned int counter = counterBegin();

	// createPlayers
    memcpy(packetString+counter, &createPlayers, sizeof(unsigned int));
    counter += sizeof(unsigned int);
}

void ProtCreatePlayers::restInformation()
{
	unsigned int counter = counterBegin();

	// createPlayers
	memcpy(&createPlayers, packetString+counter, sizeof(unsigned int));
	counter += sizeof(unsigned int);
}