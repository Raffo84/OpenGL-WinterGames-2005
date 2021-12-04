#pragma once
#include <cmath>
#ifdef _DEBUG
#include <crtdbg.h>
#endif

class stylePoints
{
public:
	stylePoints(void);
	~stylePoints(void);

	void __fastcall incPoints(float timeDiff);
	void __fastcall incAngle(float angleDiff);
	void __fastcall incHeight(float height);
	void __fastcall incExtraPoints(long);
	int  __fastcall getCurrentMultiplicator() const;
	int  __fastcall getCurrentPoints() const;
	bool __fastcall isNewMultiplicator();
	inline float	getMaxHeight() const { return maxHeight; }
	void updatePoints(bool playerHasCrashed = false);
	long getTotalPoints() const;
	inline int isActive() const { return active; }
	int isActive(float timeDiff);
	void reset();

	static const int	NOT_ACTIVE		= 0x0;
	static const int	ACTIVE_COUNTING = 0x1;
	static const int	ACTIVE_DISPLAY  = 0x2;
	static const float	pointsPerSec;
	static const float	pointsPerMeter;
	static const float	minHeightForPoints;

protected:
	float	currentAngle;
	float	lastAngle;
	float	current;
	float	maxHeight;
	long	bonusPoints;
	long	total;
	bool	newMult;
	int		active;
	float	angleMultiplicator;
			// alle 90° steigt der Multiplikator, also
			//	*1 bis 90°
			//	*2 von 90°  bis 179°
			//	*3 von 180° bis 360°
			// und so weiter.
	float	displayDelay;
};
