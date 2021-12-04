#pragma once
#include "networkpacket.h"

#include <iostream>
#include <cstring>
#include "../WinterGames/define.h"

#include "../coolLib/Color.h"

class ProtInitInformation :
	public NetworkPacket
{
public:
	ProtInitInformation(void);
	~ProtInitInformation(void);

	void giveInformation(unsigned int newPlayerNum, const char *newName, int newAvatarType, Color newHair, Color newSkin, Color newShirt, Color newGloves, Color newTrousers, Color newBoots);

	char* getName();
	int getAvatarType();
	Color getHair();
	Color getSkin();
	Color getShirt();
	Color getGloves();
	Color getTrousers();
	Color getBoots();

protected:
	char *name;
	int avatarType;
	Color hair;
	Color skin;
	Color shirt;
	Color gloves;
	Color trousers;
	Color boots;

	void restString();
	void restInformation();
};
