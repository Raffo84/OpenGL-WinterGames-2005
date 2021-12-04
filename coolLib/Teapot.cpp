#include "teapot.h"

Teapot::Teapot(void) {
	Point3D* point = new Point3D(Vector3D(0, 0, 0), Color(1.0, 1.0, 1.0));
	size = 1;
	setAllPoints(point, 1);
	delete point;

	generateDisplayList();
}

Teapot::Teapot(float newSize) {
	Point3D* point = new Point3D(Vector3D(0, 0, 0), Color(1.0, 1.0, 1.0));
	size = newSize;
	setAllPoints(point, 1);
	delete point;

	generateDisplayList();
}

Teapot::Teapot(Color* newCol, float newSize) {
	Point3D* point = new Point3D(Vector3D(0, 0, 0), *newCol);
	size = newSize;
	setAllPoints(point, 1);
	delete point;

	generateDisplayList();
}

Teapot::~Teapot(void)
{
	glDeleteLists(dlIndex, 1);
}

void Teapot::generateDisplayList() {
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
		activateTexture();
		m_points[0].color();
		glutSolidTeapot(size);
	glEndList();
}

Vector3D* Teapot::getCenter() const {
	return new Vector3D(m_points[0].getPosition().x, m_points[0].getPosition().y, m_points[0].getPosition().z);
}

//void Teapot::move(float x, float y, float z) {
//	Point3D point;
//	for (iterate(); getNextPoint(&point);) {
//		point = point + Vector3D(x, y, z);
//		setCurrentPoint(point);
//	}
//}