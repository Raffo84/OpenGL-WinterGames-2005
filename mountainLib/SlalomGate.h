#pragma once

#include "Actor.h"
#include "../coolLib/Vector3D.h"
#include "../coolLib/Color.h"
#include "../coolLib/GL/glut.h"
#include "../coolLib/Matrix3D.h"
#include "../WinterGames/define.h"

class SlalomGate : public Actor
{
public:
	SlalomGate(bool isActive = false);
	~SlalomGate(void);

	int  getType() const { return ACTOR_TYPE_TARGET; }
	int  __fastcall isActive() const;
	void __fastcall activate(bool newActive = true);
	inline float __fastcall getTimeOut() const { return timeOut; }
	void setTimeOut(float newTimeOut) { timeOut = newTimeOut; }
	void setColor(Color col, bool isGateCol = true);
	void setIndicator(unsigned int newDL) { indicatorDL = newDL; }
	void __fastcall update(float timeDiff);
	void render() const;

	bool __fastcall operator&&(Actor* other);

protected:
	bool	active;
	Color	gateCol, indicatorCol;
	float	timeOut;

private:
	unsigned int myDL, indicatorDL;

	void generateDL();
	void renderHelpCylinder(float height = 1.0f) const;
};
