#include ".\ProtLastFallSpeed.h"

ProtLastFallSpeed::ProtLastFallSpeed(void)
{
	prot = PROT_LAST_FALL_SPEED_ID;
}

ProtLastFallSpeed::~ProtLastFallSpeed(void)
{
}

void ProtLastFallSpeed::giveInformation(unsigned int newPlayerNum, float newLastFallSpeed)
{
	playerNum = newPlayerNum;
	lastFallSpeed = newLastFallSpeed;
	createString();
}

float ProtLastFallSpeed::getLastFallSpeed()
{
	return lastFallSpeed;
}

void ProtLastFallSpeed::restString()
{
	unsigned int counter = counterBegin();

	// lastFallingRate
    memcpy(packetString+counter, &lastFallSpeed, sizeof(float));
    counter += sizeof(float);
}

void ProtLastFallSpeed::restInformation()
{
	unsigned int counter = counterBegin();

	// lastFallingRate
	memcpy(&lastFallSpeed, packetString+counter, sizeof(float));
	counter += sizeof(float);
}
