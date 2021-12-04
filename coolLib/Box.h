#pragma once

#include "Shape3D.h"
#include "Point3D.h"

class Box : public Shape3D
{
public:
	Box(void);
	Box(float height, float width, float depth);
	Box(float height, float width, float depth, Point3D positionFLB);
	Box(Point3D frontLeftBottom, Point3D frontLeftTop, Point3D frontRightTop, Point3D frontRightBottom, Point3D backRightBottom, Point3D backRightTop, Point3D backLeftTop, Point3D backLeftBottom);
	~Box(void);

	Vector3D* getCenter() const;

protected:
	void createPoints(float height, float width, float depth, Point3D positionFLB);
	void generateDisplayList();

private:

};
