#include ".\protversion.h"

ProtVersion::ProtVersion(void)
{
	prot = PROT_VERSION_ID;
}

ProtVersion::~ProtVersion(void)
{
}

unsigned int ProtVersion::getVersion()
{
	return version;
}

void ProtVersion::giveInformation(unsigned int newPlayerNum, unsigned int newVersion)
{
	playerNum = newPlayerNum;
	version = newVersion;
	createString();
}

void ProtVersion::restString()
{
	unsigned int counter = counterBegin();

	// version
    memcpy(packetString+counter, &version, sizeof(unsigned int));
    counter += sizeof(unsigned int);
}

void ProtVersion::restInformation()
{
	unsigned int counter = counterBegin();

	// version
	memcpy(&version, packetString+counter, sizeof(unsigned int));
	counter += sizeof(unsigned int);
}