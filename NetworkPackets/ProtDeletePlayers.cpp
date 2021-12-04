#include "protdeleteplayers.h"

ProtDeletePlayers::ProtDeletePlayers(void)
{
	prot = PROT_DELETE_PLAYERS_ID;
}

ProtDeletePlayers::~ProtDeletePlayers(void)
{
}

void ProtDeletePlayers::giveInformation(unsigned int newPlayerNum, unsigned int newDeletePlayers)
{
	playerNum = newPlayerNum;
	deletePlayers = newDeletePlayers;
	createString();
}

unsigned int ProtDeletePlayers::getDeletePlayers()
{
	return deletePlayers;
}

void ProtDeletePlayers::restString()
{
	unsigned int counter = counterBegin();

	// createPlayers
    memcpy(packetString+counter, &deletePlayers, sizeof(unsigned int));
    counter += sizeof(unsigned int);
}

void ProtDeletePlayers::restInformation()
{
	unsigned int counter = counterBegin();

	// createPlayers
	memcpy(&deletePlayers, packetString+counter, sizeof(unsigned int));
	counter += sizeof(unsigned int);
}
