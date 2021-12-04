#pragma once
#include "../coolLib/Shape3D.h"
#include "../coolLib/Vector3D.h"
#include "hQuad.h"
#include "../WinterGames/define.h"

#include "../coolLib/GL/glut.h"
#include <vector>

class Matterhorn :
	public Shape3D
{
public:
	Matterhorn(void);
	Matterhorn(unsigned int* newQuadRenderList, unsigned int* newQuadRenderNumber);
	~Matterhorn(void);

	Vector3D* getCenter() const;
	void getCornerPoints(Vector3D* returnPosA, Vector3D* returnPosB, Vector3D* returnPosC, Vector3D* returnPosD) const;
	void moveToCoords(Vector3D newPos);

	bool loadTexture(const char* fileName) { return loadTexture(fileName, -1); }
	bool loadTexture(const char* fileName, int index);

	void renderWithQuads(Vector3D player);
	float getHeightOverGround(Vector3D checkPoint);
	Vector3D getNormal(Vector3D checkPoint);

#ifdef _DEBUG
	void printQuadsClipped();
#endif

protected:
	vector<CAMP::Bitmap*> textureArray;
	hQuad quads[QUADS_NUM];
	unsigned int* quadRenderList;
	unsigned int* quadRenderNumber;

	unsigned int findQuadForPoint(Vector3D checkPoint);
	void createHorn();
	void activateTexture() const { activateTexture(0); }
	void activateTexture(int index) const;
	void generateDisplayList();
	void generateQuads();
	void clippingTest(Vector3D player);

	unsigned int neighbourCorrect(int current);
	unsigned int* getNeighbours(unsigned int current);

	void calcViewVolumePlanes();
	float planeEqs[6][4];
};
