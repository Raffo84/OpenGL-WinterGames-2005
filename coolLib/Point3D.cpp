#include "Point3D.h"

Point3D::Point3D(void)
{
	pos = Vector3D();
	col = Color();
	norm = Vector3D();
}

Point3D::Point3D(Point3D* rhs)
{
	pos = rhs->pos;
	col = rhs->col;
	norm = rhs->norm;
}

Point3D::Point3D(Vector3D newPos, Color newCol)
{
	pos = newPos;
	col = newCol;
	norm = Vector3D();
}

Point3D::Point3D(Vector3D newPos, Vector3D newNorm, Color newCol)
{
	pos = newPos;
	col = newCol;
	norm = newNorm;
}

Point3D::~Point3D(void)
{
}

void Point3D::vertex() const {
	glVertex3f(pos.x, pos.y, pos.z);
}

void Point3D::color() const {
	col.switchToColor();
}

void Point3D::normal() const {
	if(!norm.isNull()) {
		glNormal3f(norm.x, norm.y, norm.z);
	}
}

void Point3D::vertexColor() const
{
	color();
	vertex();
}

void Point3D::vertexNormal() const {
	normal();
	vertex();
}
void Point3D::vertexNormalColor() const {
	color();
	normal();
	vertex();
}
Point3D Point3D::operator+(Point3D other)
{
	return Point3D(pos + other.pos, norm, col);
}

Point3D Point3D::operator+(Vector3D other) {
	return Point3D(pos + other, norm, col);
}

/*Point3D Point3D::operator+=(Point3D other) {
	return *this + other;
}*/

Point3D Point3D::operator-(Point3D other)
{
	return Point3D(pos - other.pos, norm, col);
}

Point3D Point3D::operator-(Vector3D other) {
	return Point3D(pos - other, norm, col);
}

Point3D Point3D::operator*(Point3D other)
{
	return Point3D(pos * other.pos, norm, col);
}

Point3D Point3D::operator*(float scalar)
{
	return Point3D(pos * scalar, norm, col);
}
