#include "pyramid.h"

Pyramid::Pyramid(void)
{
	Point3D* pointArray = new Point3D[4];

	Point3D pointZero = Point3D(0, 0, 0);
	pointArray[0] = pointZero;
	pointArray[1] = pointZero;
	pointArray[2] = pointZero;
	pointArray[3] = pointZero;

	setAllPoints(pointArray, 4);

	delete [] pointArray;
}

Pyramid::Pyramid(Point3D pointA, Point3D pointB, Point3D pointC, Point3D pointD)
{
	Point3D* pointArray = new Point3D[4];

	pointArray[0] = pointA;
	pointArray[1] = pointB;
	pointArray[2] = pointC;
	pointArray[3] = pointD;

	setAllPoints(pointArray, 4);

	delete [] pointArray;
}

Pyramid::~Pyramid(void)
{
}

void Pyramid::render()
{
	Point3D front = m_points[0];
	Point3D left = m_points[1];
	Point3D right = m_points[2];
	Point3D top = m_points[3];

	glBegin(GL_TRIANGLE_FAN);

	glTexCoord2f(0.0, 1.0);		top.vertexNormalColor();
	glTexCoord2f(0.0, 0.0);		left.vertexNormalColor();
	glTexCoord2f(1.0, 0.0);		right.vertexNormalColor();
	glTexCoord2f(1.0, 1.0);		front.vertexNormalColor();
	glTexCoord2f(1.0, 0.0);		left.vertexNormalColor();
	
	glEnd ();

	// Bottom plate

	glBegin(GL_TRIANGLES);

	glTexCoord2f(0.0, 0.0);		left.vertexNormalColor();
	glTexCoord2f(0.0, 1.0);		right.vertexNormalColor();
	glTexCoord2f(0.5, 0.5);		front.vertexNormalColor();
	
	glEnd ();
}
