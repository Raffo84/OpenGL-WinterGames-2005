#pragma once

#include "Matrix3D.h"
#include "Vector3D.h"

class MatrixHom
{
public:
	MatrixHom(void);
	MatrixHom(Vector3D, Vector3D, Vector3D);
	MatrixHom(Vector3D, Vector3D, Vector3D, Vector3D);
	MatrixHom(float*);
	~MatrixHom(void);

	void createIdentity();
	void transpone();
	float* getValues() const;

	Vector3D  operator*(Vector3D other);
	MatrixHom operator*(MatrixHom other);

private:
	//float values[16];
	float* values;
};
