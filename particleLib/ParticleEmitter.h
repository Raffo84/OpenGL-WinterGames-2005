#pragma once

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>

#include "Particle.h"
#include "../coolLib/Vector3D.h"
#include "../coolLib/Point3D.h"
#include "../mountainLib/Matterhorn.h"
#include "quad.h"
#include "../coolLib/GL/glut.h"
#include "../coolTex/Bitmap.h"
#include "../WinterGames/define.h"

class ParticleEmitter
{
public:
	ParticleEmitter(float* newDensity, Vector3D* newHeight, Vector3D* newSpeed, Vector3D* newWind, Vector3D* newMovement, Matterhorn* newHorn, unsigned int* newQuadRenderList, unsigned int* newQuadRenderNumber);
	~ParticleEmitter(void);
	
	void __fastcall update(float timeDiff); // is called each new frame
	void printParticleNumber();

protected:
	void generateDisplayListRaindrop();
	void generateDisplayListSnowflake();

	Quad quads[QUADS_NUM]; // border points are the floor

	float* density;
	Vector3D* height; // starting of the Particles
	Vector3D* speed; // from top to floor: units (from axis) per frame
	Vector3D* wind; // wind is Vector3D for modification in all axis
	Vector3D* movement;
	Matterhorn* horn;
	float timeDiffSum;
	int run;
	unsigned int* quadRenderList;
	unsigned int* quadRenderNumber;

	unsigned int dlIndexRaindrop;
	unsigned int dlIndexSnowflake;

	Vector3D* speedFaktorRaindrop; // faktor that rain is faster/slower than speed
	Vector3D* speedFaktorSnowflake; // faktor that snow is faster/slower than speed
	Vector3D* varianzRaindrop;
	Vector3D* varianzSnowflake;
	
private:

};

