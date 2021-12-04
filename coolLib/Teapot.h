#pragma once

#include "Shape3D.h"
#include "Color.h"
#include "Point3D.h"

class Teapot :
	public Shape3D
{
public:
	Teapot(void);
	Teapot(float newSize);
	Teapot(Color* newCol, float newSize = 1);
	~Teapot(void);

	Vector3D* getCenter() const;

protected:
	void generateDisplayList();

private:
	float size;
};
