#include ".\protfinaltime.h"

ProtFinalTime::ProtFinalTime(void)
{
	prot = PROT_FINAL_TIME_ID;
}

ProtFinalTime::~ProtFinalTime(void)
{
}

void ProtFinalTime::giveInformation(unsigned int newPlayerNum, double newFinalTime)
{
	playerNum = newPlayerNum;
	finalTime = newFinalTime;
	createString();
}

double ProtFinalTime::getFinalTime()
{
	return finalTime;
}

void ProtFinalTime::restString()
{
	unsigned int counter = counterBegin();

	// finalTime
    memcpy(packetString+counter, &finalTime, sizeof(double));
    counter += sizeof(double);
}

void ProtFinalTime::restInformation()
{
	unsigned int counter = counterBegin();

	// finalTime
	memcpy(&finalTime, packetString+counter, sizeof(double));
	counter += sizeof(double);
}
