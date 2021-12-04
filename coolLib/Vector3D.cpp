#include "Vector3D.h"

Vector3D::Vector3D(void) {
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

Vector3D::Vector3D(Vector3D* copy) {
	x = copy->x;
	y = copy->y;
	z = copy->z;
}

Vector3D::Vector3D(Vector3D u, Vector3D v) {
	// Kreuzprodukt
	x = u.z * v.y - u.y * v.z;
	y = u.x * v.z - u.z * v.x;
	z = u.y * v.x - u.x * v.y;
}
Vector3D::Vector3D(float newX, float newY, float newZ) {
	x = newX;
	y = newY;
	z = newZ;
}

Vector3D::~Vector3D(void) {
}

/**
 *	calculates the angle between *this Vector and the *other Vector
 *	and returns it.
 *	The value ist between 0 and 180 degrees
 */
float Vector3D::getAngle(Vector3D other) const {
	float cosPhi;
	// Scalarprodukt
	cosPhi = x * other.x + y * other.y + z * other.z;
	cosPhi /= (this->length() * other.length());
	if(cosPhi > 1.0f)
		cosPhi = 1.0f;	// correct some mathematical inaccuracies
	float phi = acos(cosPhi);
	phi = phi * 180.0f / PI;
	return phi;
}

void Vector3D::normalize() {
	if(!isNull()) {
		// get length
		float length = sqrt(x*x + y*y + z*z);

		// normalize
		x /= length;
		y /= length;
		z /= length;
	}
}

void Vector3D::clear() {
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

void Vector3D::operator=(Vector3D rhs) {
	if(&rhs == this)
		return;
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
}

bool Vector3D::operator==(Vector3D rhs) {
	return (x == rhs.x && y == rhs.y && z == rhs.z);
}

bool Vector3D::operator!=(Vector3D rhs) {
	return (x != rhs.x || y != rhs.y || z != rhs.z);
}

Vector3D Vector3D::operator+(Vector3D rhs) {
	return Vector3D(x + rhs.x, y + rhs.y, z + rhs.z);
}

void Vector3D::operator+=(Vector3D rhs) {
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
}
Vector3D Vector3D::operator-(Vector3D rhs) {
	return Vector3D(x - rhs.x, y - rhs.y, z - rhs.z);
}

void Vector3D::operator-=(Vector3D rhs) {
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
}

Vector3D Vector3D::operator*(Vector3D rhs) {
	return Vector3D(x * rhs.x, y * rhs.y, z * rhs.z);
}

Vector3D Vector3D::operator*(float scalar) {
	return Vector3D(x * scalar, y * scalar, z * scalar);
}

void Vector3D::operator*=(float scalar) {
	x *= scalar;
	y *= scalar;
	z *= scalar;
}

Vector3D Vector3D::operator/(float scalar) {
	return Vector3D(x / scalar, y / scalar, z / scalar);
}

Vector3D Vector3D::operator%(float mod) {
	float tmpX, tmpY, tmpZ;
	tmpX = x;
	tmpY = y;
	tmpZ = z;
	if(mod < 0)
		mod *= -1;
	if(mod == 0)
		return NULL;
	while(tmpX > mod)
		tmpX -= mod;
	while(tmpY > mod)
		tmpY -= mod;
	while(tmpZ > mod)
		tmpZ -= mod;

	return Vector3D(tmpX, tmpY, tmpZ);
}

//Vector3D Vector3D::operator*(Matrix3D mat) {
//#ifdef _DEBUG
//	std::cout << "Achtung: Vector3D::operator*(Matri3D) benutzt!\n";
//#endif
//
//	return (mat * this);
//}

void Vector3D::print() const {
	std::cout << "(" << x << "/" << y << "/" << z << ")";
}
