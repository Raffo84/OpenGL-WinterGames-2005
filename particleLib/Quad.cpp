#include ".\quad.h"

Quad::Quad(void)
{
}

Quad::Quad(Vector3D pointA, Vector3D pointB, Vector3D pointC, Vector3D pointD, int newType, unsigned int newDlIndexRaindrop, unsigned int newDlIndexSnowflake, Vector3D* newSpeedFaktorRaindrop, Vector3D* newSpeedFaktorSnowflake, float* newDensity, Vector3D* newHeight, Vector3D* newSpeed, Vector3D* newWind, Vector3D* newMovement, int newQuadNumber, Matterhorn *newHorn, Vector3D* newVarianzRaindrop, Vector3D* newVarianzSnowflake)
{
	//pointList = Vector3D[4];
	pointList[0] = pointA;
	pointList[1] = pointB;
	pointList[2] = pointC;
	pointList[3] = pointD;
	type = newType;
	dlIndexRaindrop = newDlIndexRaindrop;
	dlIndexSnowflake = newDlIndexSnowflake;
	speedFaktorRaindrop = newSpeedFaktorRaindrop;
	speedFaktorSnowflake = newSpeedFaktorSnowflake;
	varianzRaindrop = newVarianzRaindrop;
	varianzSnowflake = newVarianzSnowflake;
	density = newDensity;
	densityHelper = 0;
	height = newHeight;
	speed = newSpeed;
	wind = newWind;
	movement = newMovement;
	quadNumber = newQuadNumber;
	horn = newHorn;
	initialized = false;

	calcSize();
	updateMinX();
	updateMaxX();
	updateMinZ();
	updateMaxZ();
}

Quad::~Quad(void)
{
}

void Quad::render()
{
	for (unsigned int i = 0; i < activeParticles.size(); i++) {
        glPushMatrix();
        glTranslatef(activeParticles[i]->position.x, activeParticles[i]->position.y, activeParticles[i]->position.z);
        if(glIsList(activeParticles[i]->useDLindex)) glCallList(activeParticles[i]->useDLindex);
        glPopMatrix();
	}
}

void Quad::init()
{
	float totalTime = abs(height->y/speed->y);
	createNewParticles(totalTime);
	initialized = true;
}

void Quad::sinkParticles(float timeDiff)
{
	if (!initialized) init();
	// sink all particles acording to speed (done one time per frame)
	for (unsigned int i=0; i < activeParticles.size(); i++) {
		if (activeParticles[i]->varTime == 0.0f) {
            float xVar = activeParticles[i]->varianz->x;
			float yVar = activeParticles[i]->varianz->y;
			float zVar = activeParticles[i]->varianz->z;
			activeParticles[i]->currentVarianz.x = (-xVar) + ((xVar - (-xVar)) * rand()/(RAND_MAX+1.0f));
			activeParticles[i]->currentVarianz.y = (-yVar) + ((yVar - (-yVar)) * rand()/(RAND_MAX+1.0f));
			activeParticles[i]->currentVarianz.z = (-zVar) + ((zVar - (-zVar)) * rand()/(RAND_MAX+1.0f));
		}
        activeParticles[i]->position.x += ((speed->x * activeParticles[i]->speedFaktor->x + wind->x + activeParticles[i]->currentVarianz.x) * timeDiff);
        activeParticles[i]->position.y += ((speed->y * activeParticles[i]->speedFaktor->y + wind->y + activeParticles[i]->currentVarianz.y) * timeDiff);
        activeParticles[i]->position.z += ((speed->z * activeParticles[i]->speedFaktor->z + wind->z + activeParticles[i]->currentVarianz.z) * timeDiff);

		activeParticles[i]->varTime += timeDiff;
		if (activeParticles[i]->varTime >= TIME_TILL_CHANGE_VARIANZ) activeParticles[i]->varTime = 0.0f;
	}
}

void Quad::deleteOldParticles()
{
	// one time per frame: delete particles under bottom
	for (unsigned int i=0; i < activeParticles.size(); i++) {
		if ((horn->getHeightOverGround(activeParticles[i]->position)) < 0.0f) {
			inactiveParticles.push_back(activeParticles[i]);
			activeParticles[i] = activeParticles[activeParticles.size()-1];
			activeParticles.pop_back();
			i--;
		}
	}
}

void Quad::createNewParticles(float timeDiff)
{
	float randomX;
	float randomZ;
	float maxSink = speed->y * timeDiff;

	float numberAbs = (((size * *density) / abs(height->y/speed->y)) * timeDiff) + densityHelper;
	densityHelper = numberAbs - (int)numberAbs;
	int number = (int)numberAbs;
	for (int i = 0; i < number; i++) {
		randomX = minX.x + ((maxX.x - minX.x) * rand()/(RAND_MAX+1.0f));
		randomZ = minZ.z + ((maxZ.z - minZ.z) * rand()/(RAND_MAX+1.0f));
		float highY = -horn->getHeightOverGround(Vector3D(randomX, 0.0f, randomZ)) + height->y;
		float lowY = highY + maxSink;
		float y = lowY + ((highY - lowY) * rand()/(RAND_MAX+1.0f));
		doCreateParticle(randomX, y, randomZ);
	}
}

void Quad::doCreateParticle(float x, float y, float z)
{
	reactivateParticle(x, y, z);
	int last = (int)activeParticles.size() - 1;
	int tempType = type;

	if (tempType == 3) {
		int i = rand()%2;
		if (i == 0) tempType = 1;
		else tempType = 2;
	}

	if (tempType == 1) {
        activeParticles[last]->useDLindex = dlIndexRaindrop;
        activeParticles[last]->speedFaktor = speedFaktorRaindrop;
		activeParticles[last]->varianz = varianzRaindrop;
	}
	if (tempType == 2) {
        activeParticles[last]->useDLindex = dlIndexSnowflake;
        activeParticles[last]->speedFaktor = speedFaktorSnowflake;
		activeParticles[last]->varianz = varianzSnowflake;
	}
}

void Quad::reactivateParticle(float x, float y, float z)
{
	if (inactiveParticles.size() == 0) inactiveParticles.push_back(new Particle());
	activeParticles.push_back(inactiveParticles[inactiveParticles.size() - 1]);
	inactiveParticles.pop_back();

	unsigned int last = (unsigned int)activeParticles.size() - 1;
	activeParticles[last]->position.x = x;
	activeParticles[last]->position.y = y;
	activeParticles[last]->position.z = z;
	activeParticles[last]->varTime = 0.0f;
}

int Quad::numberActive()
{
	return (int)activeParticles.size();
}

int Quad::numberInactive()
{
	return (int)inactiveParticles.size();
}

void Quad::calcSize()
{
	float area = 0;
	for (int i=1; i < 4; i++) {
		area += (pointList[i-1].x * pointList[i].z) - (pointList[i].x * pointList[i-1].z);
	}
	area += (pointList[3].x * pointList[0].z) - (pointList[0].x * pointList[3].z);
	size = abs(area / 2);
}

void Quad::updateMinX()
{
	minX = pointList[0];
	for (int i=0; i < 4; i++) {
		if (minX.x > pointList[i].x) minX = pointList[i];
	}
}

void Quad::updateMaxX()
{
	maxX = pointList[0];
	for (int i=0; i < 4; i++) {
		if (maxX.x < pointList[i].x) maxX = pointList[i];
	}
}

void Quad::updateMinZ()
{
	minZ = pointList[0];
	for (int i=0; i < 4; i++) {
		if (minZ.z > pointList[i].z) minZ = pointList[i];
	}
}

void Quad::updateMaxZ()
{
	maxZ = pointList[0];
	for (int i=0; i < 4; i++) {
		if (maxZ.z < pointList[i].z) maxZ = pointList[i];
	}
}

