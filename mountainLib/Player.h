#pragma once

#include "Actor.h"
#include "../coolLib/Vector3D.h"
#include "../coolLib/Matrix3D.h"
#include "Avatar.h"
#include "Carver.h"
#include "Boarder.h"
#include "Matterhorn.h"
#include "../WinterGames/define.h"

class Player :
	public Actor
{
public:
	Player(void);
	Player(Player* copy);
	~Player(void);

	int getType() const { return ACTOR_TYPE_PERSON; }
	virtual int getPlayerType() const = 0;
	float getLOD() const { return (myChar)?(myChar->getLOD()):(0.0f); }
	void  setLOD(float l) { if(myChar) myChar->setLOD(l); }
	Vector3D getHeading() const { return heading; }
	void setHeading(Vector3D newHeading) { heading = newHeading; }
	Vector3D getAirHeading() const { return airHeading; }
	void setAirHeading(Vector3D newAirHeading) { airHeading = newAirHeading; }
	std::string getName() const { return name; }
	void  setName(const std::string newName);
	void  setJumpBonus(float);
	float __fastcall getTotalAngle() const { return totalAngle; }
	inline float __fastcall getSpeed() const { return speed; }
	void  __fastcall setSpeed(float s);
	inline bool __fastcall isFlying() const { return (flying >= PLAYER_FLYING_TRESHOLD) && (speed >= 1.4f); }
	void __fastcall setCamRot(Vector3D cam) { camRot = cam; }
	inline bool __fastcall isStopped() const { return stopped; }
	float __fastcall getCurrentJumpBonus() const;
	bool isNewBonus();
	virtual void setReadyFlag(bool newFlag) { lanReady = newFlag; }
	bool getReadyFlag() const { return lanReady; }

#ifdef _DEBUG
	virtual void fullReport() const;	// only to use in DEBUG mode!
#endif

	virtual void __fastcall stop(float delay = 0.0f);
	virtual void __fastcall restart(float delay = 0.0f);
	virtual void switchPlayerChar();
	virtual void crashAgainst(Vector3D otherPos);
	virtual void jump();
	virtual void turnPlayer(float angle);
	virtual void accPlayer(float factor);
	virtual void update(float timeDiff, Vector3D groundNormal);
	virtual void __fastcall fall(float timeDiff, float heightOverGround);
	virtual void render() const;

	Avatar*		myChar;

protected:
	Vector3D	heading, airHeading;
	Vector3D	camRot;
	std::string	name;
	float	speed;
	float	lastFallSpeed;
	float	flying;
	float	jumpBonus;
	int		useJumpBonus, oldUseJumpBonus;
	bool	newBonus;
	float	totalAngle;
	bool	stopped;
	float	stopTimer;
	float	standUpTime;
	bool	lanReady;
};
