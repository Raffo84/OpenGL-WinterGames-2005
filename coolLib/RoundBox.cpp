#include "roundbox.h"

RoundBox::RoundBox(void) {
	createPoints(1.0f, 1.0f, 1.0f, Point3D(Vector3D(-0.5f, -0.5f, -0.5f), Color(1.0f, 1.0f, 1.0f)));
	generateDisplayList();
}

RoundBox::RoundBox(float height, float width, float depth) {
	createPoints(height, width, depth, Point3D(0, 0, 0));
	generateDisplayList();
}

RoundBox::RoundBox(float height, float width, float depth, Point3D positionFLB) {
	createPoints(height, width, depth, positionFLB);
	generateDisplayList();
}

RoundBox::RoundBox(Point3D pointA, Point3D pointB, Point3D pointC, Point3D pointD, Point3D pointE, Point3D pointF, Point3D pointG, Point3D pointH) {
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

RoundBox::~RoundBox(void)
{
	glDeleteLists(dlIndex, 1);
}

void RoundBox::generateDisplayList() {
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

		glBegin(GL_QUAD_STRIP);

		glTexCoord2f(0.0, 0.0);		backLeftTop.vertexNormalColor();
		glTexCoord2f(0.0, 1.0);		backLeftBottom.vertexNormalColor();
		glTexCoord2f(1.0, 1.0);		backRightTop.vertexNormalColor();
		glTexCoord2f(1.0, 0.0);		backRightBottom.vertexNormalColor();
		glTexCoord2f(0.0, 1.0);		frontRightTop.vertexNormalColor();
		glTexCoord2f(0.0, 0.0);		frontRightBottom.vertexNormalColor();
		glTexCoord2f(1.0, 1.0);		frontLeftTop.vertexNormalColor();
		glTexCoord2f(1.0, 0.0);		frontLeftBottom.vertexNormalColor();
		glTexCoord2f(0.0, 0.0);		backLeftTop.vertexNormalColor();
		glTexCoord2f(0.0, 1.0);		backLeftBottom.vertexNormalColor();

		glEnd ();

		glBegin(GL_QUADS);

		// Top plate
		glTexCoord2f(0.0, 0.0);		frontLeftTop.vertexNormalColor();
		glTexCoord2f(0.0, 1.0);		frontRightTop.vertexNormalColor();
		glTexCoord2f(1.0, 1.0);		backRightTop.vertexNormalColor();
		glTexCoord2f(1.0, 0.0);		backLeftTop.vertexNormalColor();

		// Bottom plate
		glTexCoord2f(0.0, 0.0);		backLeftBottom.vertexNormalColor();
		glTexCoord2f(0.0, 1.0);		frontLeftBottom.vertexNormalColor();
		glTexCoord2f(1.0, 1.0);		frontRightBottom.vertexNormalColor();
		glTexCoord2f(1.0, 0.0);		backRightBottom.vertexNormalColor();
		
		glEnd ();
	glEndList();
}

void RoundBox::createPoints(float height, float width, float depth, Point3D positionFLB) {
	Point3D* pointArray = new Point3D[8];
	Vector3D pos0, pos1, pos2, pos3, pos4, pos5, pos6, pos7;
	Vector3D norm;

	// Coordinates and Color (used from positionFLB)
	pointArray[0] = positionFLB + Vector3D(0, 0, 0);
	pointArray[1] = positionFLB + Vector3D(0, height, 0);
	pointArray[2] = positionFLB + Vector3D(width, height, 0);
	pointArray[3] = positionFLB + Vector3D(width, 0, 0);

	pointArray[4] = positionFLB + Vector3D(width, 0, depth);
	pointArray[5] = positionFLB + Vector3D(width, height, depth);
	pointArray[6] = positionFLB + Vector3D(0, height, depth);
	pointArray[7] = positionFLB + Vector3D(0, 0, depth);

	// for the Normals
	pos0 = pointArray[0].getPosition();
	pos1 = pointArray[1].getPosition();
	pos2 = pointArray[2].getPosition();
	pos3 = pointArray[3].getPosition();

	pos4 = pointArray[4].getPosition();
	pos5 = pointArray[5].getPosition();
	pos6 = pointArray[6].getPosition();
	pos7 = pointArray[7].getPosition();

	norm = Vector3D(pos1-pos3, pos7-pos3);
	norm.normalize();
	pointArray[0].setNormal(norm);
	norm = Vector3D(pos2-pos0, pos6-pos0);
	norm.normalize();
	pointArray[1].setNormal(norm);
	norm = Vector3D(pos5-pos3, pos1-pos3);
	norm.normalize();
	pointArray[2].setNormal(norm);
	norm = Vector3D(pos0-pos2, pos4-pos2);
	norm.normalize();
	pointArray[3].setNormal(norm);

	norm = Vector3D(pos3-pos5, pos7-pos5);
	norm.normalize();
	pointArray[4].setNormal(norm);
	norm = Vector3D(pos6-pos4, pos2-pos4);
	norm.normalize();
	pointArray[5].setNormal(norm);
	norm = Vector3D(pos1-pos7, pos5-pos7);
	norm.normalize();
	pointArray[6].setNormal(norm);
	norm = Vector3D(pos4-pos6, pos0-pos6);
	norm.normalize();
	pointArray[7].setNormal(norm);

	setAllPoints(pointArray, 8);

	delete [] pointArray;
}

Vector3D* RoundBox::getCenter() const {
	float x, y, z;
	x = m_points[3].getPosition().x - m_points[0].getPosition().x;
	y = m_points[1].getPosition().y - m_points[0].getPosition().y;
	z = m_points[7].getPosition().z - m_points[0].getPosition().z;
	return new Vector3D(x, y, z);
}