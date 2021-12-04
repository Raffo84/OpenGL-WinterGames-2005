#pragma once
#include "Player.h"
#include "stylePoints.h"
#include "define.h"
#include <cstdlib>
#include <ctime>
#include <fstream>

#define AI_MAX_WAIT_FOR_TURBOBOOST	10.0f

class AIPlayer :
	public Player
{
public:
	AIPlayer(void);
	AIPlayer(std::string name);
	AIPlayer(Player* copy);
	~AIPlayer(void);

	int getPlayerType() const { return PLAYER_TYPE_AI; }
	void setGameMode(int newMode);
	void setNameColor(Color* newCol) { nameCol = newCol; }
	void setStylePoints(stylePoints* rhs) { myPoints = rhs; }

	void update(float timeDiff, Vector3D groundNormal);
	void __fastcall fall(float timeDiff, float heightOverGround);
	void __fastcall stop(float delay = 0.0f);
	void __fastcall restart(float delay = 0.0f);
	void render() const;
	virtual void setReadyFlag(bool newFlag) { lanReady = true; }

private:
	bool	isSet;
	float	currentRot;
	int		rotSign;
	float	oldSpeed;
	int		zeroSpeedCheck;
	int		gameMode;
	float	waitForTurboBoost;
	Color*	nameCol;
	float	oldAcc;
	float	lastHeight;
	stylePoints*	myPoints;
	float	toRotate;

	void init();
};
