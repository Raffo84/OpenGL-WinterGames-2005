#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter(float* newDensity, Vector3D* newHeight, Vector3D* newSpeed, Vector3D* newWind, Vector3D* newMovement, Matterhorn* newHorn, unsigned int* newQuadRenderList, unsigned int* newQuadRenderNumber)
{
	generateDisplayListRaindrop();
	generateDisplayListSnowflake();

	speedFaktorRaindrop = new SPEED_FAKTOR_RAINDROP;
	speedFaktorSnowflake = new SPEED_FAKTOR_SNOWFLAKE;
	varianzRaindrop = new VARIANZ_RAINDROP;
	varianzSnowflake = new VARIANZ_SNOWFLAKE;

	density = newDensity;
	height = newHeight;
	speed = newSpeed;
	wind = newWind;
	movement = newMovement;
	horn = newHorn;
	timeDiffSum = 0.0f;
	run = 0;
	quadRenderList = newQuadRenderList;
	quadRenderNumber = newQuadRenderNumber;

	Point3D points[(M_LENGTH+1)*(M_LENGTH+1)];
	Point3D point;
	horn->iterate();
	int i = 0;
	while(horn->getNextPoint(&point)) {
		points[i] = point;
		i++;
	}

	int counter = 0;
	//float xCoord = -9999999.0f;
	//float yCoord = -9999999.0f;
	//float zCoord = -9999999.0f;
	for(unsigned int z = 0; z < M_LENGTH; z++) {
		for(unsigned int x = 0; x < M_LENGTH; x++) {
			Vector3D pA = points[x + z*(M_LENGTH+1)].getPosition();
			Vector3D pB = points[x + (z+1)*(M_LENGTH+1)].getPosition();
			Vector3D pC = points[(x+1) + (z+1)*(M_LENGTH+1)].getPosition();
			Vector3D pD = points[(x+1) + z*(M_LENGTH+1)].getPosition();
			int type = 0;
			if(points[x + z*(M_LENGTH+1)].getColor() == Color(1.0f, 1.0f, 1.0f)) type = 2;		// snow
			else if(points[x + z*(M_LENGTH+1)].getColor() == Color(0.7f, 0.7f, 0.7f)) type = 3;	// rock
			else type = 1; // grass
			quads[counter] = Quad(pA, pB, pC, pD, type, dlIndexRaindrop, dlIndexSnowflake, speedFaktorRaindrop, speedFaktorSnowflake, density, height, speed, wind, movement, counter, horn, varianzRaindrop, varianzSnowflake);
			counter++;
			//if (pA.x > zCoord) {
			//	xCoord = pA.x;
			//	yCoord = pA.y;
			//	zCoord = pA.z;
			//}
			//if (pB.x > zCoord) {
			//	xCoord = pB.x;
			//	yCoord = pB.y;
			//	zCoord = pB.z;
			//}
			//if (pC.x > zCoord) {
			//	xCoord = pC.x;
			//	yCoord = pC.y;
			//	zCoord = pC.z;
			//}
			//if (pD.x > zCoord) {
			//	xCoord = pD.x;
			//	yCoord = pD.y;
			//	zCoord = pD.z;
			//}
		}
	}
	//std::cout << "X: " << xCoord << " // Y: " << yCoord << " // Z: " << zCoord << std::endl;

	// random init
	srand((unsigned int)time(NULL));
}

ParticleEmitter::~ParticleEmitter(void)
{
	delete speedFaktorRaindrop;
	delete speedFaktorSnowflake;
	delete varianzRaindrop;
	delete varianzSnowflake;
}

void __fastcall ParticleEmitter::update(float timeDiff)
{
	timeDiffSum += timeDiff;
	run++;
	if ((timeDiff < MIN_TIMEDIFF_TO_UPDATE) || (run >= MAX_RUNS_TILL_UPDATE)) {
		for (unsigned int i = 0; i < *quadRenderNumber; i++) {
            quads[quadRenderList[i]].sinkParticles(timeDiffSum);
            quads[quadRenderList[i]].createNewParticles(timeDiffSum);
            quads[quadRenderList[i]].deleteOldParticles();
		}
		timeDiffSum = 0.0f;
		run = 0;
	}

	// always render everything
	glPushMatrix();
	for (unsigned int i=0; i < *quadRenderNumber; i++) {
		quads[quadRenderList[i]].render();
	}
	glPopMatrix();
}

void ParticleEmitter::printParticleNumber()
{
	int active = 0;
    for (unsigned int i = 0; i < *quadRenderNumber; i++) {
        active += quads[quadRenderList[i]].numberActive();
	}

	std::cout << "Visisble: " << active << std::endl;
}

void ParticleEmitter::generateDisplayListRaindrop()
{
	if(glIsList(dlIndexRaindrop))
		glDeleteLists(dlIndexRaindrop, 1);
	else {
		dlIndexRaindrop = glGenLists(1);
		if(dlIndexRaindrop == 0) {
#ifdef _DEBUG
			std::cout << "\n\t!!!!!\nKrass, keine DL's mehr frei?!?\n\t!!!!!\n\n";
#endif
			return;
		}
	}

	float height = 0.1f;
	float width = 0.06f;
	glNewList(dlIndexRaindrop, GL_COMPILE);
		GLboolean isEnTex = glIsEnabled(GL_TEXTURE_2D);
		GLboolean isEnBFC = glIsEnabled(GL_CULL_FACE);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_CULL_FACE);

		glBegin(GL_QUADS);
		glColor3f(0.0f, 0.0f, 0.7f);
		glVertex3f (-width/2, height/2, 0.0f);
		glVertex3f (-width/2, -height/2, 0.0f);
		glVertex3f (width/2, -height/2, 0.0f);
		glVertex3f (width/2, height/2, 0.0f);
		glEnd();

		if(isEnTex) glEnable(GL_TEXTURE_2D);
		if(isEnBFC) glEnable(GL_CULL_FACE);
	glEndList();
}

void ParticleEmitter::generateDisplayListSnowflake()
{
	if(glIsList(dlIndexSnowflake))
		glDeleteLists(dlIndexSnowflake, 1);
	else {
		dlIndexSnowflake = glGenLists(1);
		if(dlIndexSnowflake == 0) {
#ifdef _DEBUG
			std::cout << "\n\t!!!!!\nKrass, keine DL's mehr frei?!?\n\t!!!!!\n\n";
#endif
			return;
		}
	}

	float height = 0.15f;
	float width = 0.15f;

	glNewList(dlIndexSnowflake, GL_COMPILE);
		GLboolean isEnTex = glIsEnabled(GL_TEXTURE_2D);
		GLboolean isEnBFC = glIsEnabled(GL_CULL_FACE);
		GLboolean isEnLig = glIsEnabled(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_CULL_FACE);
		glDisable(GL_LIGHTING);

		glBegin(GL_TRIANGLE_FAN);
		glColor3f(1.0f, 1.0f, 1.0f);
		glNormal3f(0.0f, 0.0f, 2.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);

		glVertex3f(0.0f, height/2.0f, 0.0f);
		glVertex3f(width/10.0f, height/6.0f, 0.0f);
		glVertex3f(width/2.0f, height/3.0f, 0.0f);
		glVertex3f(width/6.0f, 0.0f, 0.0f);
		glVertex3f(width/2.0f, -height/3.0f, 0.0f);
		glVertex3f(width/10.0f, -height/6.0f, 0.0f);

		glVertex3f(0.0f, -height/2.0f, 0.0f);
		glVertex3f(-width/10.0f, -height/6.0f, 0.0f);
		glVertex3f(-width/2.0f, -height/3.0f, 0.0f);
		glVertex3f(-width/6.0f, 0.0f, 0.0f);
		glVertex3f(-width/2.0f, height/3.0f, 0.0f);
		glVertex3f(-width/10.0f, height/6.0f, 0.0f);
		glVertex3f(0.0f, height/2.0f, 0.0f);

		glEnd();

		// same from other side (x-z changed)
		glBegin(GL_TRIANGLE_FAN);
		glNormal3f(2.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);

		glVertex3f(0.0f, height/2.0f, 0.0f);
		glVertex3f(0.0f, height/6.0f, width/10.0f);
		glVertex3f(0.0f, height/3.0f, width/2.0f);
		glVertex3f(0.0f, 0.0f, width/6.0f);
		glVertex3f(0.0f, -height/3.0f, width/2.0f);
		glVertex3f(0.0f, -height/6.0f, width/10.0f);

		glVertex3f(0.0f, -height/2.0f, 0.0f);
		glVertex3f(0.0f, -height/6.0f, -width/10.0f);
		glVertex3f(0.0f, -height/3.0f, -width/2.0f);
		glVertex3f(0.0f, 0.0f, -width/6.0f);
		glVertex3f(0.0f, height/3.0f, -width/2.0f);
		glVertex3f(0.0f, height/6.0f, -width/10.0f);
		glVertex3f(0.0f, height/2.0f, 0.0f);

		glEnd();

		if(isEnTex) glEnable(GL_TEXTURE_2D);
		if(isEnBFC) glEnable(GL_CULL_FACE);
		if(isEnLig) glEnable(GL_LIGHTING);
	glEndList();
}