#include ".\protstylepoints.h"

ProtStylePoints::ProtStylePoints(void)
{
	prot = PROT_STYLE_POINTS_ID;
}

ProtStylePoints::~ProtStylePoints(void)
{
}

void ProtStylePoints::giveInformation(unsigned int newPlayerNum, long newStylePoints)
{
	playerNum = newPlayerNum;
	stylePoints = newStylePoints;
	createString();
}

long ProtStylePoints::getStylePoints()
{
	return stylePoints;
}

void ProtStylePoints::restString()
{
	unsigned int counter = counterBegin();

	// stylePoints
    memcpy(packetString+counter, &stylePoints, sizeof(long));
    counter += sizeof(long);
}

void ProtStylePoints::restInformation()
{
	unsigned int counter = counterBegin();

	// stylePoints
	memcpy(&stylePoints, packetString+counter, sizeof(long));
	counter += sizeof(long);
}
