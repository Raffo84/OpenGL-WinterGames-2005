#pragma once

#include "Color.h"
#include "Vector3D.h"
#include "GL/glut.h"

class Point3D
{
public:
	Point3D(void);
	Point3D(Point3D* rhs);
	Point3D(Vector3D newPos, Color newCol = Color());
	Point3D(Vector3D newPos, Vector3D newNorm, Color newCol = Color());
	~Point3D(void);

	void vertex() const;
	void color() const;
	void normal() const;
	void vertexColor() const;
	void vertexNormal() const;
	void vertexNormalColor() const;

	void setPosition(Vector3D newPos) { pos = newPos; }
	Vector3D getPosition() const { return pos; }
	void setNormal(Vector3D newNorm) { norm = newNorm; }
	Vector3D getNormal() const { return norm; }
	void setColor(Color newCol) { col = newCol; }
	Color getColor() const { return col; }

	Point3D operator+ (Point3D other);
	Point3D operator+(Vector3D other);
	//Point3D operator+= (Point3D other);
	Point3D operator- (Point3D other);
	Point3D operator-(Vector3D other);
	Point3D operator* (Point3D other);
	Point3D operator* (float factor);
	//Point3D operator* (Matrix3D matrix);	// --> Vector3D

protected:
	Vector3D pos;
	Vector3D norm;
	Color col;


private:
	
};
