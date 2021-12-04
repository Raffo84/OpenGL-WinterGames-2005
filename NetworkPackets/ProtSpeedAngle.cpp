#include ".\protspeedangle.h"

ProtSpeedAngle::ProtSpeedAngle(void)
{
	prot = PROT_SPEED_ANGLE_ID;
}

ProtSpeedAngle::~ProtSpeedAngle(void)
{
}

void ProtSpeedAngle::giveInformation(unsigned int newPlayerNum, float newSpeed, float newAngle)
{
	playerNum = newPlayerNum;
	speed = newSpeed;
	angle = newAngle;
	createString();
}

float ProtSpeedAngle::getSpeed()
{
	return speed;
}

float ProtSpeedAngle::getAngle()
{
	return angle;
}

void ProtSpeedAngle::restString()
{
	unsigned int counter = counterBegin();

	// speed
    memcpy(packetString+counter, &speed, sizeof(float));
    counter += sizeof(float);

    // angle
	memcpy(packetString+counter, &angle, sizeof(float));
    counter += sizeof(float);
}

void ProtSpeedAngle::restInformation()
{
	unsigned int counter = counterBegin();

	// speed
	memcpy(&speed, packetString+counter, sizeof(float));
	counter += sizeof(float);

	// angle
	memcpy(&angle, packetString+counter, sizeof(float));
	counter += sizeof(float);
}