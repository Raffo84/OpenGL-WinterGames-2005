#pragma once

#include <iostream>

#include "../coolLib/Vector3D.h"
#include "../coolLib/GL/glut.h"
#include "../WinterGames/define.h"

class Particle
{
public:
	Particle(void);
	~Particle(void);

	unsigned int useDLindex;
	Vector3D position;
	Vector3D *speedFaktor;
	Vector3D *varianz;
	Vector3D currentVarianz;
	float varTime;

protected:

private:

};
