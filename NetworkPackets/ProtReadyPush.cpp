#include ".\protreadypush.h"

ProtReadyPush::ProtReadyPush(void)
{
	prot = PROT_READY_PUSH_ID;
}

ProtReadyPush::~ProtReadyPush(void)
{
}

void ProtReadyPush::giveInformation(unsigned int newPlayerNum)
{
	playerNum = newPlayerNum;
	createString();
}

void ProtReadyPush::restString()
{
	unsigned int counter = counterBegin();
}

void ProtReadyPush::restInformation()
{
	unsigned int counter = counterBegin();
}