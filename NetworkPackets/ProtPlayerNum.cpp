#include ".\protplayernum.h"

ProtPlayerNum::ProtPlayerNum(void)
{
	prot = PROT_PLAYER_NUM_ID;
}

ProtPlayerNum::~ProtPlayerNum(void)
{
}

void ProtPlayerNum::giveInformation(unsigned int newPlayerNum)
{
	playerNum = newPlayerNum;
	createString();
}

void ProtPlayerNum::restString()
{
}

void ProtPlayerNum::restInformation()
{
}