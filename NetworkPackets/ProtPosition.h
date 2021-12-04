#pragma once
#include "networkpacket.h"

#include <iostream>
#include <cstring>
#include "../WinterGames/define.h"

#include "../coolLib/Vector3D.h"

class ProtPosition :
	public NetworkPacket
{
public:
	ProtPosition(void);
	~ProtPosition(void);

	void giveInformation(unsigned int playerNum, Vector3D newPosition);

	Vector3D getPosition();

protected:
	Vector3D position;

	void restString();
	void restInformation();
};
