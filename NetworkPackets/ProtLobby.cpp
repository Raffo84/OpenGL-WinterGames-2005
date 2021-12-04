#include ".\protlobby.h"

ProtLobby::ProtLobby(void)
{
	prot = PROT_LOBBY_ID;
}

ProtLobby::~ProtLobby(void)
{
}

void ProtLobby::giveInformation(unsigned int newPlayerNum)
{
	playerNum = newPlayerNum;
	createString();
}

void ProtLobby::restString()
{
	unsigned int counter = counterBegin();
}

void ProtLobby::restInformation()
{
	unsigned int counter = counterBegin();
}