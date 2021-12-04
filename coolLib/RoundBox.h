#pragma once
#include "Shape3D.h"

class RoundBox :
	public Shape3D
{
public:
	RoundBox(void);
	RoundBox(float height, float width, float depth);
	RoundBox(float height, float width, float depth, Point3D positionFLB);
	RoundBox(Point3D frontLeftBottom, Point3D frontLeftTop, Point3D frontRightTop, Point3D frontRightBottom, Point3D backRightBottom, Point3D backRightTop, Point3D backLeftTop, Point3D backLeftBottom);
	~RoundBox(void);

	Vector3D* getCenter() const;

protected:
	void createPoints(float height, float width, float depth, Point3D positionFLB);
	void generateDisplayList();
};
