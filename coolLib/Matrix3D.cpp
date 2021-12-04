#include "Matrix3D.h"

Matrix3D::Matrix3D(void) {
	//*this = Matrix3D(	Vector3D(0, 0, 0),
	//					Vector3D(0, 0, 0),
	//					Vector3D(0, 0, 0) );
	columns[0] = Vector3D(0, 0, 0);
	columns[1] = Vector3D(0, 0, 0);
	columns[2] = Vector3D(0, 0, 0);
}

Matrix3D::Matrix3D(Vector3D a, Vector3D b, Vector3D c) {
	//columns = new Vector3D[3];
	columns[0] = a;
	columns[1] = b;
	columns[2] = c;
}

Matrix3D::~Matrix3D(void) {
//	delete [] columns;
}

void Matrix3D::createRotationMatrix(float phi, float theta, float eta) {
	Vector3D a, b, c;
	phi   = phi   / 180.0f * MATRIX_PI;
	theta = theta / 180.0f * MATRIX_PI;
	eta   = eta   / 180.0f * MATRIX_PI;

	a.x = cos(eta)*cos(theta);
	a.y = sin(eta)*cos(phi) + cos(eta)*sin(theta)*sin(phi);
	a.z = sin(eta)*sin(phi) - cos(eta)*sin(theta)*cos(phi);

	b.x =-sin(eta)*cos(theta);
	b.y = cos(eta)*cos(phi) - sin(eta)*sin(theta)*sin(phi);
    b.z = cos(eta)*sin(phi) + sin(eta)*sin(theta)*cos(phi);

	c.x = sin(theta);
	c.y =-cos(theta)*sin(phi);
	c.z = cos(theta)*cos(phi);

	columns[0] = a;
	columns[1] = b;
	columns[2] = c;
}

void Matrix3D::createScaleMatrix(float factorX, float factorY, float factorZ) {
	columns[0] = Vector3D(factorX, 0, 0);
	columns[1] = Vector3D(0, factorY, 0);
	columns[2] = Vector3D(0, 0, factorZ);
}

void Matrix3D::createIdentity() {
	columns[0] = Vector3D(1, 0, 0);
	columns[1] = Vector3D(0, 1, 0);
	columns[2] = Vector3D(0, 0, 1);
}

void Matrix3D::transpone() {
	Vector3D a, b, c;

	a.x = columns[0].x;
	a.y = columns[1].x;
	a.z = columns[2].x;

	b.x = columns[0].y;
	b.y = columns[1].y;
	b.z = columns[2].y;

	c.x = columns[0].z;
	c.y = columns[1].z;
	c.z = columns[2].z;

	columns[0] = a;
	columns[1] = b;
	columns[2] = c;
}

Vector3D Matrix3D::getColumn(int i) {
	if(i < 3)
		return columns[i];

	return Vector3D(0, 0, 0);
}

Vector3D Matrix3D::operator*(Vector3D other) {
	Vector3D res;
	res.x = res.y = res.z = 0;

	res = res + (columns[0] * other.x);
	res = res + (columns[1] * other.y);
	res = res + (columns[2] * other.z);

	return res;
}

Matrix3D Matrix3D::operator*(Matrix3D other) {
	Matrix3D res;
	res.columns[0] = *this * other.columns[0];
	res.columns[1] = *this * other.columns[1];
	res.columns[2] = *this * other.columns[2];

	return res;
}