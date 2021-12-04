#pragma once

#include <cmath>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>

#include "Particle.h"
#include "../coolLib/Vector3D.h"
#include "../mountainLib/Matterhorn.h"
#include "../WinterGames/define.h"

class Quad
{
public:
	Quad(void);
	Quad(Vector3D pointA, Vector3D pointB, Vector3D pointC, Vector3D pointD, int newType, unsigned int newDlIndexRaindrop, unsigned int newDlIndexSnowflake, Vector3D* newSpeedFaktorRaindrop, Vector3D* newSpeedFaktorSnowflake, float* newDensity, Vector3D* newHeight, Vector3D* newSpeed, Vector3D* newWind, Vector3D* newMovement, int newQuadNumber, Matterhorn *newHorn, Vector3D* newVarianzRaindrop, Vector3D* newVarianzSnowflake);
	~Quad(void);

	void sinkParticles(float timeDiff); // particles sink acording to speed per frame
	void createNewParticles(float timeDiff); // every frame we create particles
	void deleteOldParticles(); // delete particles that are under bottom
	void render();
	void init();

	Vector3D pointList[4];


	int numberActive();
	int numberInactive();

protected:
	std::vector<Particle*> activeParticles;
	std::vector<Particle*> inactiveParticles;

	void calcSize();

	unsigned int dlIndexRaindrop;
	unsigned int dlIndexSnowflake;
	Vector3D* speedFaktorRaindrop;
	Vector3D* speedFaktorSnowflake;
	Vector3D* varianzRaindrop;
	Vector3D* varianzSnowflake;
	float* density;
	float densityHelper;
	Vector3D* height; // starting of the Particles
	Vector3D* speed; // from top to floor: units (from axis) per frame
	Vector3D* acceleration; // acceleration
	Vector3D* wind;
	Vector3D* movement;
	int quadNumber;
	Matterhorn *horn;
	bool initialized;

	int type;
	float size;

	void updateMinX();
	void updateMaxX();
	void updateMinZ();
	void updateMaxZ();

	Vector3D minX;
	Vector3D maxX;
	Vector3D minZ;
	Vector3D maxZ;

	void doCreateParticle(float x, float y, float z); // sets particle acording to particleType
	void reactivateParticle(float x, float y, float z);

private:

};
