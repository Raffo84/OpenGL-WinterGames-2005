#include ".\protcrash.h"

ProtCrash::ProtCrash(void)
{
	prot = PROT_CRASH_ID;
}

ProtCrash::~ProtCrash(void)
{
}

void ProtCrash::giveInformation(unsigned int newPlayerNum, unsigned int newCrashedAgainst, char newCrashType)
{
	playerNum = newPlayerNum;
	crashedAgainst = newCrashedAgainst;
	crashType = newCrashType;
	createString();
}

unsigned int ProtCrash::getCrashedAgainst()
{
	return crashedAgainst;
}

char ProtCrash::getCrashType()
{
	return crashType;
}

void ProtCrash::restString()
{
	unsigned int counter = counterBegin();

	// crashedAgainst
    memcpy(packetString+counter, &crashedAgainst, sizeof(unsigned int));
    counter += sizeof(unsigned int);
	// crashType
    memcpy(packetString+counter, &crashType, sizeof(char));
    counter += sizeof(char);
}

void ProtCrash::restInformation()
{
	unsigned int counter = counterBegin();

	// crashedAgainst
	memcpy(&crashedAgainst, packetString+counter, sizeof(unsigned int));
	counter += sizeof(unsigned int);
	// crashType
	memcpy(&crashType, packetString+counter, sizeof(char));
	counter += sizeof(char);
}
