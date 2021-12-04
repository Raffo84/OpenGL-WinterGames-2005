#pragma once

#include <iostream>
#include <sstream>
#include <string>

#include "../coolLib/Point3D.h"
#include "../coolLib/Vector3D.h"
#include "../WinterGames/define.h"

class hQuad
{
public:
	hQuad(void);
	hQuad(Point3D pointA, Point3D pointB, Point3D pointC, Point3D pointD, int newDLindex);
	~hQuad(void);

	void render(unsigned int num);
	float getHeightOfPoint(float x, float z);
	Vector3D getNormal(Vector3D checkPoint);
	bool isInQuad(Vector3D checkPoint);

	Point3D pointList[4];
	float testPointList[8][3];
	int clipped;
	void culled(float (*planeEqs)[6][4]);

protected:
	int dlIndex;

	void calcNormals();
	void updateMinX();
	void updateMaxX();
	void updateMinZ();
	void updateMaxZ();
	void updateZ();

	Vector3D minX;
	Vector3D maxX;
	Vector3D minZ;
	Vector3D maxZ;
	float mXsmall;
	float mZsmall;
	float mXhigh;
	float mZhigh;
	Vector3D normalSmall;
	Vector3D normalHigh;

};
