#include ".\protheading.h"

ProtHeading::ProtHeading(void)
{
	prot = PROT_HEADING_ID;
}

ProtHeading::~ProtHeading(void)
{
}

void ProtHeading::giveInformation(unsigned int newPlayerNum, Vector3D newHeading, Vector3D newAirHeading)
{
	playerNum = newPlayerNum;
	heading = newHeading;
	airHeading = newAirHeading;
	createString();
}

Vector3D ProtHeading::getHeading()
{
	return heading;
}

Vector3D ProtHeading::getAirHeading()
{
	return airHeading;
}

void ProtHeading::restString()
{
	unsigned int counter = counterBegin();

	// heading.x
	memcpy(packetString+counter, &heading.x, sizeof(float));
    counter += sizeof(float);
	// heading.y
	memcpy(packetString+counter, &heading.y, sizeof(float));
    counter += sizeof(float);
	// heading.z
	memcpy(packetString+counter, &heading.z, sizeof(float));
    counter += sizeof(float);

	// airHeading.x
	memcpy(packetString+counter, &airHeading.x, sizeof(float));
    counter += sizeof(float);
	// airHeading.y
	memcpy(packetString+counter, &airHeading.y, sizeof(float));
    counter += sizeof(float);
	// airHeading.z
	memcpy(packetString+counter, &airHeading.z, sizeof(float));
    counter += sizeof(float);
}

void ProtHeading::restInformation()
{
	unsigned int counter = counterBegin();

	// heading.x
	memcpy(&heading.x, packetString+counter, sizeof(float));
	counter += sizeof(float);
	// heading.y
	memcpy(&heading.y, packetString+counter, sizeof(float));
	counter += sizeof(float);
	// heading.z
	memcpy(&heading.z, packetString+counter, sizeof(float));
	counter += sizeof(float);

	// airHeading.x
	memcpy(&airHeading.x, packetString+counter, sizeof(float));
	counter += sizeof(float);
	// airHeading.y
	memcpy(&airHeading.y, packetString+counter, sizeof(float));
	counter += sizeof(float);
	// airHeading.z
	memcpy(&airHeading.z, packetString+counter, sizeof(float));
	counter += sizeof(float);
}