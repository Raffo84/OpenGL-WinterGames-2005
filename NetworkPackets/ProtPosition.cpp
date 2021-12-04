#include ".\protposition.h"

ProtPosition::ProtPosition(void)
{
	prot = PROT_POSITION_ID;
}

ProtPosition::~ProtPosition(void)
{
}

void ProtPosition::giveInformation(unsigned int newPlayerNum, Vector3D newPosition)
{
	playerNum = newPlayerNum;
	position = newPosition;
	createString();
}

Vector3D ProtPosition::getPosition()
{
	return position;
}

void ProtPosition::restString()
{
	unsigned int counter = counterBegin();

	// position
    memcpy(packetString+counter, &(position.x), sizeof(float));
    counter += sizeof(float);
	memcpy(packetString+counter, &(position.y), sizeof(float));
    counter += sizeof(float);
	memcpy(packetString+counter, &(position.z), sizeof(float));
    counter += sizeof(float);
}

void ProtPosition::restInformation()
{
	unsigned int counter = counterBegin();

	// position
	memcpy(&(position.x), packetString+counter, sizeof(float));
	counter += sizeof(float);
	memcpy(&(position.y), packetString+counter, sizeof(float));
	counter += sizeof(float);
	memcpy(&(position.z), packetString+counter, sizeof(float));
	counter += sizeof(float);
}