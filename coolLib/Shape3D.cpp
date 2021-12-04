
#include "Shape3D.h"

Shape3D::Shape3D(void)
{
	texture = new CAMP::Bitmap();

	//objectMatrix = new float[16];
	//for(int x = 0; x < 4; x++)		// initialize to identity
	//	for(int y = 0; y < 4; y++)
	//		objectMatrix[4*x + y] = (x==y)?(1.0f):(0.0f);
}

Shape3D::~Shape3D(void)
{
	//delete [] objectMatrix;
	delete texture;
}

void Shape3D::iterate()
{
	m_iterator = m_points.begin();
}

bool Shape3D::getNextPoint(Point3D *returnValue)
{
	if (m_iterator != m_points.end())
	{
        *returnValue = *m_iterator;
		m_iterator++;
		//std::cout << &*m_iterator << std::endl;
        return true;
	}
	else
	{
		return false;
	}
}

bool Shape3D::setCurrentPoint(Point3D newPoint)
{
	bool isFirstPoint = false;
	if (m_iterator == m_points.begin()) isFirstPoint = true;

	if(!isFirstPoint) m_iterator--;
	*m_iterator = newPoint;
	if (!isFirstPoint) m_iterator++;
	return true;
}

bool Shape3D::setNextPoint(Point3D newPoint)
{
	if (m_iterator != m_points.end())
	{
		*m_iterator = newPoint;
        return true;
	}
	return false;
}

void Shape3D::printPoints() {
	std::cout << "printing Points...\n";

	iterate();
	Point3D current;
	while(getNextPoint(&current)) {
		std::cout << "(" << current.getPosition().x << "; " << current.getPosition().y << "; " << current.getPosition().z << ")\n";
	}

	std::cout << "...end\n\n";
}

void Shape3D::render() const
{
	glPushMatrix();
#ifdef USING_MATRICES_FOR_MOVEMENT
	glTranslatef(movement.x, movement.y, movement.z);
#endif
#ifdef USING_MATRICES_FOR_ROTATION
	glRotatef(rotation.x, 1.0f, 0.0f, 0.0f);
	glRotatef(rotation.y, 0.0f, 1.0f, 0.0f);
	glRotatef(rotation.z, 0.0f, 0.0f, 1.0f);
#endif
	//glMultMatrixf(objectMatrix);

	if(glIsList(dlIndex))
		glCallList(dlIndex);

	glPopMatrix();
}

void Shape3D::renderNormals() {
	Point3D  point;
	Vector3D normal;

	glPushMatrix();
#ifdef USING_MATRICES_FOR_MOVEMENT
	glTranslatef(movement.x, movement.y, movement.z);
#endif
#ifdef USING_MATRICES_FOR_ROTATION
	glRotatef(rotation.x, 1.0f, 0.0f, 0.0f);
	glRotatef(rotation.y, 0.0f, 1.0f, 0.0f);
	glRotatef(rotation.z, 0.0f, 0.0f, 1.0f);
#endif

	iterate();
	glBegin(GL_LINES);
	while(getNextPoint(&point)) {
		normal = point.getPosition() + point.getNormal();
		glColor3f(1.0f, 0.0f, 0.0f);
		point.vertex();
		glVertex3f(normal.x, normal.y, normal.z);
	}
	glEnd();

	glPopMatrix();
}

void Shape3D::move(float x, float y, float z) {
#ifndef USING_MATRICES_FOR_MOVEMENT
	Point3D point;
	for (iterate(); getNextPoint(&point);) {
		point = point + Vector3D(x, y, z);
		setCurrentPoint(point);
	}
#else
	movement += Vector3D(x, y, z);

	//glPushMatrix();
	//glLoadMatrixf(objectMatrix);
	//glTranslatef(x, y, z);
	//glGetFloatv(GL_MODELVIEW_MATRIX, objectMatrix);
	//glPopMatrix();
#endif

}

void Shape3D::scale(float factor) {
	Point3D point;
	Vector3D origin;

	iterate();
	getNextPoint(&point);	// get first point and move it to origin, to minimize scaling errors
	origin = point.getPosition();

	for (iterate(); getNextPoint(&point);) {
		point = point - origin;
		point = point * factor;
		point = point + origin;
		setCurrentPoint(point);
	}

	generateDisplayList();
}

void Shape3D::rotate(float phi, float theta, float eta) {
#ifndef USING_MATRICES_FOR_ROTATION
	Matrix3D mat = Matrix3D();
	Point3D point;
	Vector3D origin;
	mat.createRotationMatrix(phi, theta, eta);
	origin = getCenter();
	move(-origin.x, -origin.y, -origin.z);

	for(iterate(); getNextPoint(&point); ) {
		point.setPosition(mat * point.getPosition());
		//setNextPoint(point);
		setCurrentPoint(point);
	}

	move(origin.x, origin.y, origin.z);
#else
	rotation += Vector3D(phi, theta, eta);
	rotation = rotation % 360.0f;

	//glPushMatrix();
	//glLoadMatrixf(objectMatrix);
	//glRotatef((float)phi,   1, 0, 0);
	//glRotatef((float)theta, 0, 1, 0);
	//glRotatef((float)eta,   0, 0, 1);
	//glGetFloatv(GL_MODELVIEW_MATRIX, objectMatrix);
	//glPopMatrix();
#endif
}

bool Shape3D::loadTexture(const char* file) {
	if(texture->load(file) <= 0)
		return false;
	generateDisplayList();
	return true;
}

void Shape3D::activateTexture() const {
	int texWidth, texHeight;
	texWidth  = texture->getWidth();
	texHeight = texture->getHeight();
	glTexImage2D(GL_TEXTURE_2D, 0, 3, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texture->getData());
}

void Shape3D::setAllPoints(Point3D* pointArray, unsigned int length) {
	for(unsigned int i = 0; i < length; i++) {
		m_points.push_back(pointArray[i]);
	}
}
