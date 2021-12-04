#include "box.h"

Box::Box(void) {
	createPoints(1.0f, 1.0f, 1.0f, Point3D(Vector3D(-0.5f, -0.5f, -0.5f), Color(1.0f, 1.0f, 1.0f)));
	generateDisplayList();
}

Box::Box(float height, float width, float depth) {
	createPoints(height, width, depth, Point3D(0, 0, 0));
	generateDisplayList();
}

Box::Box(float height, float width, float depth, Point3D positionFLB) {
	createPoints(height, width, depth, positionFLB);
	generateDisplayList();
}

Box::Box(Point3D pointA, Point3D pointB, Point3D pointC, Point3D pointD, Point3D pointE, Point3D pointF, Point3D pointG, Point3D pointH) {
	Point3D* pointArray = new Point3D[8];

	pointArray[0] = pointA;
	pointArray[1] = pointB;
	pointArray[2] = pointC;
	pointArray[3] = pointD;

	pointArray[4] = pointE;
	pointArray[5] = pointF;
	pointArray[6] = pointG;
	pointArray[7] = pointH;

	setAllPoints(pointArray, 8);

	delete [] pointArray;

	generateDisplayList();
}

Box::~Box(void)
{
	glDeleteLists(dlIndex, 1);
}

void Box::generateDisplayList() {
	if(glIsList(dlIndex))
		glDeleteLists(dlIndex, 1);
	else {
		dlIndex = glGenLists(1);
		if(dlIndex == 0) {
#ifdef _DEBUG
			std::cout << "\n\t!!!!!\nKrass, keine DL's mehr frei?!?\n\t!!!!!\n\n";
#endif
			return;
		}
	}

	glNewList(dlIndex, GL_COMPILE);
		Point3D frontLeftBottom = m_points[0];
		Point3D frontLeftTop = m_points[1];
		Point3D frontRightTop = m_points[2];
		Point3D frontRightBottom = m_points[3];
		Point3D backRightBottom = m_points[4];
		Point3D backRightTop = m_points[5];
		Point3D backLeftTop = m_points[6];
		Point3D backLeftBottom = m_points[7];

		activateTexture();

		glBegin(GL_QUADS);

		// Back
		backRightTop.normal();
		glTexCoord2f(0.0, 0.0);		backLeftTop.vertexColor();
		glTexCoord2f(0.0, 1.0);		backLeftBottom.vertexColor();
		glTexCoord2f(1.0, 0.0);		backRightBottom.vertexColor();
		glTexCoord2f(1.0, 1.0);		backRightTop.vertexColor();

		// Right
		backRightBottom.normal();
		glTexCoord2f(1.0, 1.0);		backRightTop.vertexColor();
		glTexCoord2f(1.0, 0.0);		backRightBottom.vertexColor();
		glTexCoord2f(0.0, 0.0);		frontRightBottom.vertexColor();
		glTexCoord2f(0.0, 1.0);		frontRightTop.vertexColor();

		// Front
		frontRightBottom.normal();
		glTexCoord2f(0.0, 1.0);		frontRightTop.vertexColor();
		glTexCoord2f(0.0, 0.0);		frontRightBottom.vertexColor();
		glTexCoord2f(1.0, 0.0);		frontLeftBottom.vertexColor();
		glTexCoord2f(1.0, 1.0);		frontLeftTop.vertexColor();

		// Left
		frontLeftTop.normal();
		glTexCoord2f(1.0, 1.0);		frontLeftTop.vertexColor();
		glTexCoord2f(1.0, 0.0);		frontLeftBottom.vertexColor();
		glTexCoord2f(0.0, 1.0);		backLeftBottom.vertexColor();
		glTexCoord2f(0.0, 0.0);		backLeftTop.vertexColor();

		// Top
		frontRightTop.normal();
		glTexCoord2f(0.0, 0.0);		frontLeftTop.vertexColor();
		glTexCoord2f(0.0, 1.0);		frontRightTop.vertexColor();
		glTexCoord2f(1.0, 1.0);		backRightTop.vertexColor();
		glTexCoord2f(1.0, 0.0);		backLeftTop.vertexColor();

		// Bottom
		frontLeftBottom.normal();
		glTexCoord2f(0.0, 0.0);		backLeftBottom.vertexColor();
		glTexCoord2f(0.0, 1.0);		frontLeftBottom.vertexColor();
		glTexCoord2f(1.0, 1.0);		frontRightBottom.vertexColor();
		glTexCoord2f(1.0, 0.0);		backRightBottom.vertexColor();
		
		glEnd ();
	glEndList();
}

Vector3D* Box::getCenter() const {
	float x, y, z;
	x = m_points[3].getPosition().x - m_points[0].getPosition().x;
	y = m_points[1].getPosition().y - m_points[0].getPosition().y;
	z = m_points[7].getPosition().z - m_points[0].getPosition().z;
	return new Vector3D(x, y, z);
}

void Box::createPoints(float height, float width, float depth, Point3D positionFLB) {
	Point3D* pointArray = new Point3D[8];

	// Coordinates and Color (used from positionFLB)
	pointArray[0] = positionFLB + Vector3D(0, 0, 0);
	pointArray[1] = positionFLB + Vector3D(0, height, 0);
	pointArray[2] = positionFLB + Vector3D(width, height, 0);
	pointArray[3] = positionFLB + Vector3D(width, 0, 0);

	pointArray[4] = positionFLB + Vector3D(width, 0, depth);
	pointArray[5] = positionFLB + Vector3D(width, height, depth);
	pointArray[6] = positionFLB + Vector3D(0, height, depth);
	pointArray[7] = positionFLB + Vector3D(0, 0, depth);

	// Normals
	pointArray[0].setNormal(Vector3D(0.0f, -1.0f, 0.0f));	// bottom
	pointArray[1].setNormal(Vector3D(-1.0f, 0.0f, 0.0f));	// left
	pointArray[2].setNormal(Vector3D(0.0f, 1.0f, 0.0f));	// top
	pointArray[3].setNormal(Vector3D(0.0f, 0.0f, -1.0f));	// front

	pointArray[4].setNormal(Vector3D(1.0f, 0.0f, 0.0f));	// right
	pointArray[5].setNormal(Vector3D(0.0f, 0.0f, 1.0f));	// back
	//pointArray[6].setNormal(Vector3D(0.0f, 0.0f, 0.0f));	// Only 6 Normals needed for a box
	//pointArray[7].setNormal(Vector3D(0.0f, 0.0f, 0.0f));

	setAllPoints(pointArray, 8);

	delete [] pointArray;
}