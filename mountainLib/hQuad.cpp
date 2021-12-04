#include ".\hQuad.h"

hQuad::hQuad(void)
{
}

hQuad::~hQuad(void)
{
}

hQuad::hQuad(Point3D pointA, Point3D pointB, Point3D pointC, Point3D pointD, int newDLindex)
{
	pointList[0] = pointA;
	pointList[1] = pointB;
	pointList[2] = pointC;
	pointList[3] = pointD;

	updateMinX();
	updateMaxX();
	updateMinZ();
	updateMaxZ();
	updateZ();
	dlIndex = newDLindex;
	calcNormals();

	clipped = 1;

	for (unsigned int i = 0; i < 4; i++) {
		testPointList[i][0] = pointList[i].getPosition().x;
		testPointList[i][1] = pointList[i].getPosition().y;
		testPointList[i][2] = pointList[i].getPosition().z;
	}

	float x = minX.x + ((maxX.x - minX.x) / 2);
	float z = minZ.z;
	testPointList[4][0] = x;
	testPointList[4][1] = getHeightOfPoint(x, z);
	testPointList[4][2] = z;

	x = minX.x + ((maxX.x - minX.x) / 2);
	z = maxZ.z;
	testPointList[5][0] = x;
	testPointList[5][1] = getHeightOfPoint(x, z);
	testPointList[5][2] = z;

	x = minX.x;
	z = minZ.z + ((maxZ.z - minZ.z) / 2);
	testPointList[6][0] = x;
	testPointList[6][1] = getHeightOfPoint(x, z);
	testPointList[6][2] = z;

	x = maxX.x;
	z = minZ.z + ((maxZ.z - minZ.z) / 2);
	testPointList[7][0] = x;
	testPointList[7][1] = getHeightOfPoint(x, z);
	testPointList[7][2] = z;
}

void hQuad::render(unsigned int num)
{
	if(glIsList(dlIndex)) glCallList(dlIndex);
#ifdef _DEBUG
	std::stringstream oss;
	std::string quadText;
	oss << "Q: " << num;
	quadText = oss.str();
	float x = minX.x + 12.5;
	float z = minZ.z + 12.5;
	glColor3f(0.0f, 0.0f, 1.0f);
	glRasterPos3f(x + 12.5, getHeightOfPoint(x, z), z);
	GLboolean isEnTex = glIsEnabled(GL_TEXTURE_2D);
	GLboolean isEnLight = glIsEnabled(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glColor3f(0.0f, 0.0f, 1.0f);
	for(unsigned int i = 0; i < quadText.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, (int)quadText[i]);
	}
	if(isEnTex)		glEnable(GL_TEXTURE_2D);
	if(isEnLight)	glEnable(GL_LIGHTING);
	
	// show border of quad
	GLboolean isEnTex2 = glIsEnabled(GL_TEXTURE_2D);
	GLboolean isEnBFC = glIsEnabled(GL_CULL_FACE);
	GLboolean isEnLig = glIsEnabled(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);

	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(pointList[0].getPosition().x, pointList[0].getPosition().y, pointList[0].getPosition().z);
	glVertex3f(pointList[1].getPosition().x, pointList[1].getPosition().y, pointList[1].getPosition().z);
	glVertex3f(pointList[1].getPosition().x, pointList[1].getPosition().y, pointList[1].getPosition().z);
	glVertex3f(pointList[2].getPosition().x, pointList[2].getPosition().y, pointList[2].getPosition().z);
	glVertex3f(pointList[2].getPosition().x, pointList[2].getPosition().y, pointList[2].getPosition().z);
	glVertex3f(pointList[3].getPosition().x, pointList[3].getPosition().y, pointList[3].getPosition().z);
	glVertex3f(pointList[3].getPosition().x, pointList[3].getPosition().y, pointList[3].getPosition().z);
	glVertex3f(pointList[0].getPosition().x, pointList[0].getPosition().y, pointList[0].getPosition().z);
	glEnd();

	if(isEnTex2) glEnable(GL_TEXTURE_2D);
	if(isEnBFC) glEnable(GL_CULL_FACE);
	if(isEnLig) glEnable(GL_LIGHTING);
#endif
}

float hQuad::getHeightOfPoint(float x, float z)
{
	float height;

	if ((x-pointList[0].getPosition().x + z-pointList[0].getPosition().z) <= 25) {
        height = pointList[0].getPosition().y;
        height += (z - pointList[0].getPosition().z) * mZsmall;
        height += (x - pointList[0].getPosition().x) * mXsmall;
	}
	else {
		height = pointList[2].getPosition().y;
        height += (z - pointList[2].getPosition().z) * mZhigh;
        height += (x - pointList[2].getPosition().x) * mXhigh;
	}
	
	return height;
}

Vector3D hQuad::getNormal(Vector3D checkPoint)
{
	if ((checkPoint.x-pointList[0].getPosition().x + checkPoint.z-pointList[0].getPosition().z) <= 25) {
        return normalSmall;
	}
	else {
		return normalHigh;
	}
}

bool hQuad::isInQuad(Vector3D checkPoint)
{
	if ((minX.x <= checkPoint.x) && (checkPoint.x <= maxX.x)) {
		if ((minZ.z <= checkPoint.z) && (checkPoint.z <= maxZ.z)) return true;
	}
	return false;
}

void hQuad::calcNormals()
{
	Vector3D tempA;
	Vector3D tempB;

	tempA = pointList[3].getPosition() - pointList[0].getPosition();
    tempB = pointList[1].getPosition() - pointList[0].getPosition();
    normalSmall = Vector3D(tempA, tempB);
	normalSmall.normalize();

	tempA = pointList[1].getPosition() - pointList[2].getPosition();
    tempB = pointList[3].getPosition() - pointList[2].getPosition();
    normalHigh = Vector3D(tempA, tempB);
	normalHigh.normalize();
}

void hQuad::updateMinX()
{
	minX = pointList[0].getPosition();
	for (int i=0; i < 4; i++) {
		if (minX.x > pointList[i].getPosition().x) minX = pointList[i].getPosition();
	}
}

void hQuad::updateMaxX()
{
	maxX = pointList[0].getPosition();
	for (int i=0; i < 4; i++) {
		if (maxX.x < pointList[i].getPosition().x) maxX = pointList[i].getPosition();
	}
}

void hQuad::updateMinZ()
{
	minZ = pointList[0].getPosition();
	for (int i=0; i < 4; i++) {
		if (minZ.z > pointList[i].getPosition().z) minZ = pointList[i].getPosition();
	}
}

void hQuad::updateMaxZ()
{
	maxZ = pointList[0].getPosition();
	for (int i=0; i < 4; i++) {
		if (maxZ.z < pointList[i].getPosition().z) maxZ = pointList[i].getPosition();
	}
}

void hQuad::updateZ()
{
	mXsmall = (pointList[3].getPosition().y - pointList[0].getPosition().y) / (pointList[3].getPosition().x - pointList[0].getPosition().x);
	mZsmall = (pointList[1].getPosition().y - pointList[0].getPosition().y) / (pointList[1].getPosition().z - pointList[0].getPosition().z);

	mXhigh = (pointList[1].getPosition().y - pointList[2].getPosition().y) / (pointList[1].getPosition().x - pointList[2].getPosition().x);
	mZhigh = (pointList[3].getPosition().y - pointList[2].getPosition().y) / (pointList[3].getPosition().z - pointList[2].getPosition().z);
}

void hQuad::culled(float (*planeEqs)[6][4])
{
	unsigned int pointsVisible = 0;
			
    for (unsigned int i=0; i<8; i++) {
		// for each plane we check here all borderPoints
		unsigned int outOfPlane = 0;
		for (unsigned int j=0; j < 6; j++) {
			float part1 = (*planeEqs)[j][0] * testPointList[i][0];
			float part2 = (*planeEqs)[j][1] * testPointList[i][1];
			float part3 = (*planeEqs)[j][2] * testPointList[i][2];
			float part4 = (*planeEqs)[j][3];
			float distance = part1 + part2 + part3 + part4;
			if (distance <= 0.) {
				outOfPlane++;
				break;
			}
		}
#ifdef _DEBUG
		float x = testPointList[i][0];
		float z = testPointList[i][2];
		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;
		if (i == 0) {
			x += 1.5f;
			z += 1.5f;
		}
		if (i == 1) {
			x += 1.5f;
			z -= 1.5f;
		}
		if (i == 2) {
			x -= 1.5f;
			z -= 1.5f;
		}
		if (i == 3) {
			x -= 1.5f;
			z += 1.5f;
		}
		if (i == 4) z += 1.5f;
		if (i == 5) z -= 1.5f;
		if (i == 6) x += 1.5f;
		if (i == 7) x -= 1.5f;
#endif
		// testing if point is inside all planes
		if (outOfPlane == 0) {
			pointsVisible++;
#ifdef _DEBUG
			g = 1.0f;
#else
			break;
#endif
		} else {
#ifdef _DEBUG
			r = 1.0f;
#endif
		}
#ifdef _DEBUG
		// show border of quad
		GLboolean isEnTex = glIsEnabled(GL_TEXTURE_2D);
		GLboolean isEnBFC = glIsEnabled(GL_CULL_FACE);
		GLboolean isEnLig = glIsEnabled(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_CULL_FACE);
		glDisable(GL_LIGHTING);

		glPointSize(10.0f);
		glBegin(GL_POINTS);
		glColor3f(r, g, b);
		glVertex3f(x, getHeightOfPoint(x, z), z);
		glEnd();

		if(isEnTex) glEnable(GL_TEXTURE_2D);
		if(isEnBFC) glEnable(GL_CULL_FACE);
		if(isEnLig) glEnable(GL_LIGHTING);
		if (g == 1.0f) break;
#endif
	}

	if (pointsVisible >= 1) {
		clipped = 0;
	} else {
		clipped = 1;
	}
}
