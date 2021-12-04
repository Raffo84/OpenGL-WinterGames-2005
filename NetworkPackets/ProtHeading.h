#pragma once
#include "networkpacket.h"

#include <iostream>
#include <cstring>
#include "../WinterGames/define.h"

#include "../coolLib/Vector3D.h"

class ProtHeading :
	public NetworkPacket
{
public:
	ProtHeading(void);
	~ProtHeading(void);

	void giveInformation(unsigned int playerNum, Vector3D newHeading, Vector3D newAirHeading);

	Vector3D	getHeading();
	Vector3D	getAirHeading();

protected:
	Vector3D	heading;
	Vector3D	airHeading;

	void restString();
	void restInformation();
};
