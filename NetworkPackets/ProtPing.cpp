#include ".\protping.h"

ProtPing::ProtPing(void)
{
	prot = PROT_PING_ID;
}

ProtPing::~ProtPing(void)
{
}

void ProtPing::giveInformation(unsigned int newPlayerNum)
{
	playerNum = newPlayerNum;
	createString();
}

void ProtPing::restString()
{
	unsigned int counter = counterBegin();
}

void ProtPing::restInformation()
{
	unsigned int counter = counterBegin();
}