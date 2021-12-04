#pragma once

#include <cmath>
#include <iostream>
//#include "Matrix3D.h"
#ifndef PI
#define PI 3.1415926535897932384626433f
#endif

class Vector3D
{
public:
	float x, y, z;

	Vector3D(void);
	Vector3D(Vector3D* Copy);
	Vector3D(Vector3D u, Vector3D v);	// Kreuzprodukt-Konstruktor
	Vector3D(float x, float y, float z);
	~Vector3D(void);

	bool isNull() const { return (x == 0 && y == 0 && z == 0); }
	float length() const { return sqrt(x*x + y*y + z*z); }
	float getAngle(Vector3D other) const;
	void clear();
	void normalize();
	void print() const;
	char* str() const;

	void	 operator=(Vector3D);
	bool	 operator==(Vector3D);
	bool	 operator!=(Vector3D);
	Vector3D operator+(Vector3D);
	void	 operator+=(Vector3D);
	Vector3D operator-(Vector3D);
	void	 operator-=(Vector3D);
	Vector3D operator*(Vector3D);
	Vector3D operator/(float scalar);
//	Vector3D operator*(Matrix3D);
	Vector3D operator*(float Scalar);
	void	 operator*=(float scalar);
	Vector3D operator%(float);

protected:

private:

};
