#pragma once

#include "../WinterGames/define.h"

class PlayerPacketHelper
{
public:
	PlayerPacketHelper(void);
	~PlayerPacketHelper(void);

	unsigned int getLength(char prot);
};
