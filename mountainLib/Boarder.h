#pragma once
#include "Avatar.h"
#include "../WinterGames/define.h"

class Boarder :
	public Avatar
{
public:
	Boarder(void);
	Boarder(const Avatar*);
	~Boarder(void);

	int getType() const { return AVATAR_TYPE_BOARDER; }

protected:
	Color colBoard;
	void generateDisplayList();
};
