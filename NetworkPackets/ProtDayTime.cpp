#include ".\protdaytime.h"

ProtDayTime::ProtDayTime(void)
{
	prot = PROT_DAY_TIME_ID;
}

ProtDayTime::~ProtDayTime(void)
{
}

unsigned int ProtDayTime::getNight()
{
	return night;
}

void ProtDayTime::giveInformation(unsigned int newPlayerNum, unsigned int newNight)
{
	playerNum = newPlayerNum;
	night = newNight;
	createString();
}

void ProtDayTime::restString()
{
	unsigned int counter = counterBegin();

	// night
    memcpy(packetString+counter, &night, sizeof(unsigned int));
    counter += sizeof(unsigned int);
}

void ProtDayTime::restInformation()
{
	unsigned int counter = counterBegin();

	// night
	memcpy(&night, packetString+counter, sizeof(unsigned int));
	counter += sizeof(unsigned int);
}
