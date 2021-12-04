#include ".\protgametype.h"

ProtGameType::ProtGameType(void)
{
	prot = PROT_GAME_TYPE_ID;
}

ProtGameType::~ProtGameType(void)
{
}

unsigned int ProtGameType::getGameType()
{
	return gameType;
}

void ProtGameType::giveInformation(unsigned int newPlayerNum, unsigned int newGameType)
{
	playerNum = newPlayerNum;
	gameType = newGameType;
	createString();
}

void ProtGameType::restString()
{
	unsigned int counter = counterBegin();

	// gameType
    memcpy(packetString+counter, &gameType, sizeof(unsigned int));
    counter += sizeof(char);
}

void ProtGameType::restInformation()
{
	unsigned int counter = counterBegin();

	// gameType
	memcpy(&gameType, packetString+counter, sizeof(unsigned int));
	counter += sizeof(char);
}
