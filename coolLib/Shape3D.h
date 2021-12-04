#pragma once
#ifndef shape3d_h_
#define shape3d_h_

#include <iostream>
#include <vector>
#include <cmath>
#include "Point3D.h"
#include "Vector3D.h"
#include "Matrix3D.h"
#include "GL/glut.h"
#include "../coolTex/Bitmap.h"

#ifndef PI
#define PI 3.14159265358979323<84626433f
#endif

#define USING_MATRICES_FOR_ROTATION
#define USING_MATRICES_FOR_MOVEMENT

using namespace std;

class Shape3D
{
public:
	Shape3D(void);
	~Shape3D(void);

	void iterate();
	bool getNextPoint(Point3D* returnValue);
	bool setNextPoint(Point3D newPoint);
	bool setCurrentPoint(Point3D newPoint);

	void printPoints();
	void renderNormals();
	Vector3D getPosition() const { return movement; }
	Vector3D getRotation() const { return rotation; }
	virtual void render() const;
	virtual Vector3D* getCenter() const = 0;
	virtual void rotate(float phi, float theta, float eta);
	virtual void scale(float factor);
	virtual void move(float x, float y, float z);
	virtual bool loadTexture(const char* fileName);

protected:
	vector<Point3D>::iterator m_iterator;
	vector<Point3D> m_points;
	CAMP::Bitmap* texture;
	Vector3D movement, rotation;
	//float* objectMatrix;
	unsigned int dlIndex;

	virtual void activateTexture() const;
	virtual void setAllPoints(Point3D* array, unsigned int length);
	virtual void generateDisplayList() = 0;

private:

};

#endif // shape3d_h_
