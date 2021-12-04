#pragma once
#include "Actor.h"
#include "../coolLib/GL/glut.h"
#include "../coolTex/Bitmap.h"

#include "../WinterGames/define.h"

class Tree :
	public Actor
{
public:
	Tree(void);
	~Tree(void);

	int getType() const { return ACTOR_TYPE_OBSTACLE; }

	void render() const;

private:
	static unsigned int		totalNumber;
	static unsigned int		myDisplayList;
	static CAMP::Bitmap*	myTex;
	static void createMyDL();
	static void renderHelp(float, float, int);
	//static void renderMyOwnFuckingCone(float base, float height, int lod);
};
