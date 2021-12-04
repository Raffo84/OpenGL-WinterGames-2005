#include ".\proterror.h"

ProtError::ProtError(void)
{
	prot = PROT_ERROR_ID;
}

ProtError::~ProtError(void)
{
}

unsigned int ProtError::getError()
{
	return error;
}

void ProtError::giveInformation(unsigned int newPlayerNum, unsigned int newError)
{
	playerNum = newPlayerNum;
	error = newError;
	createString();
}

void ProtError::restString()
{
	unsigned int counter = counterBegin();

	// night
    memcpy(packetString+counter, &error, sizeof(unsigned int));
    counter += sizeof(unsigned int);
}

void ProtError::restInformation()
{
	unsigned int counter = counterBegin();

	// night
	memcpy(&error, packetString+counter, sizeof(unsigned int));
	counter += sizeof(unsigned int);
}