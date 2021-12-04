#pragma once

#include <cmath>
#include "Vector3D.h"
#define MATRIX_PI 3.1415926535897932384626433f

class Matrix3D
{
public:
	Matrix3D(void);
	Matrix3D(Vector3D a, Vector3D b, Vector3D c);
	~Matrix3D(void);

	void createRotationMatrix(float phi, float theta, float eta);
	void createScaleMatrix(float factorX, float factorY, float factorZ);
	void createIdentity();
	inline void createScaleMatrix(float factor) {
		createScaleMatrix(factor, factor, factor);
	}
	void transpone();
	Vector3D getColumn(int i);

	Vector3D operator*(Vector3D other);
	Matrix3D operator*(Matrix3D other);

protected:
	int size;
	Vector3D columns[3];
};
