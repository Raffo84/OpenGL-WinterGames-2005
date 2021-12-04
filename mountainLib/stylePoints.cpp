#include "stylePoints.h"

const float stylePoints::pointsPerSec = 100.0f;
const float stylePoints::pointsPerMeter = 50.0f;
const float stylePoints::minHeightForPoints = 100.0f;

stylePoints::stylePoints(void) {
	reset();
}

stylePoints::~stylePoints(void) {
}

void __fastcall stylePoints::incPoints(float timeDiff) {
	if(isActive() != ACTIVE_COUNTING) {
		current = 0.0f;
		active = ACTIVE_COUNTING;
		lastAngle = currentAngle = 0.0f;
		newMult = false;
	}
	current += timeDiff * pointsPerSec;
#ifdef _DEBUG
	_ASSERT(current < 5000);
#endif
}

void __fastcall stylePoints::incAngle(float angle) {
	lastAngle += angle;
	if(abs(lastAngle) > currentAngle) {
		int oldMult = getCurrentMultiplicator();
		currentAngle = abs(lastAngle);
		if( oldMult < getCurrentMultiplicator())
			newMult = true;
	}
}

void __fastcall stylePoints::incHeight(float height) {
	if(height > maxHeight)
		maxHeight = height;
}

void __fastcall stylePoints::incExtraPoints(long bonus) {
	bonusPoints += bonus;
	if(bonusPoints < 0)
		bonusPoints = 0;
}

int __fastcall stylePoints::getCurrentMultiplicator() const {
	float tempAngle = currentAngle;
	int mult = 1;
	while(tempAngle >= angleMultiplicator) {
		mult++;
		tempAngle -= angleMultiplicator;
	}
#ifdef _DEBUG
	_ASSERT(mult < 15);
#endif
	return mult;
}

int __fastcall stylePoints::getCurrentPoints() const {
	return (int)current;
}

bool __fastcall stylePoints::isNewMultiplicator() {
	if(newMult) {
		newMult = false;
		return true;
	}
	return false;
}

void stylePoints::updatePoints(bool hasCrashed) {
#ifdef _DEBUG
	_ASSERT(current < 5000);
#endif
	if(hasCrashed)
		current *= -1.0f;
	else
		current *= getCurrentMultiplicator();

	total += (long)current;
	if(total < 0)
		total = 0;
	//current = 0.0f;
	currentAngle = 0.0f;
	newMult = false;
	active = ACTIVE_DISPLAY;
	displayDelay = 2.0f;
}

long stylePoints::getTotalPoints() const {
	if(total < 0)
		return 0;
	else {
		long res = total;
		if(maxHeight >= minHeightForPoints) {
			res += (long)((minHeightForPoints * pointsPerMeter) / 4.0f);
			res += (long)((maxHeight - minHeightForPoints) * pointsPerMeter);
		}
		else if(maxHeight >= (minHeightForPoints / 2.0f))
			res += (long)((maxHeight - (minHeightForPoints / 2.0f)) * (pointsPerMeter / 2.0f));
		res += bonusPoints;
#ifdef _DEBUG
		_ASSERT(res < 40000);
#endif
		return res;
	}
}

int stylePoints::isActive(float timeDiff) {
	if(active == ACTIVE_DISPLAY)
		displayDelay -= timeDiff;
	if((active != ACTIVE_COUNTING) && (displayDelay < 0.0f)) {
		current = 0.0f;
		active = NOT_ACTIVE;
	}
	return active;
}

void stylePoints::reset() {
	currentAngle = 0.0f;
	lastAngle = 0.0f;
	current = 0.0f;
	maxHeight = 0.0f;
	bonusPoints = 0;
	total = 0;
	newMult = false;
	active = NOT_ACTIVE;
	//pointsPerSec = 100.0f;
	angleMultiplicator = 90.0f;
	displayDelay = 2.0f;
}

