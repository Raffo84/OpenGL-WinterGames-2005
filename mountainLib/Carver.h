#pragma once
#include "Avatar.h"
#include "../WinterGames/define.h"

class Carver :
	public Avatar
{
public:
	Carver(void);
	Carver(const Avatar*);
	~Carver(void);

	int getType() const { return AVATAR_TYPE_CARVER; }

protected:
	Color colSkis;
	void generateDisplayList();
};
