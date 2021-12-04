#pragma once
#include "networkpacket.h"

class ProtFinish :
	public NetworkPacket
{
public:
	ProtFinish(void);
	~ProtFinish(void);
};
