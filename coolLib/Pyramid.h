#pragma once

#include "Shape3D.h"
#include "Point3D.h"

class Pyramid : public Shape3D
{
public:
	Pyramid(void);
	Pyramid(Point3D front, Point3D left, Point3D right, Point3D top);
	~Pyramid(void);

	void render();

protected:

private:

};
