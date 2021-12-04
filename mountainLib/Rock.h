#pragma once
#include "Actor.h"
#include "../coolLib/GL/glut.h"
#include "../coolTex/Bitmap.h"

#include "../WinterGames/define.h"

class Rock :
	public Actor
{
public:
	Rock(void);
	~Rock(void);

	int getType() const { return ACTOR_TYPE_OBSTACLE; }

	void render() const;

private:
	static unsigned int		totalNumber;
	static unsigned int		myDisplayList;
	static CAMP::Bitmap*	myTex;
	static void createMyDL();
};
