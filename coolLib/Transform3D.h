#pragma once

#include "Matrix3D.h"
#include "Shape3D.h"
#include "Point3D.h"

class Transform3D
{
public:
	Transform3D(void);
	~Transform3D(void);

	void setShape3D(Shape3D* newShape);
	Point3D getBoundingBoxCenter();
	void translate(Point3D transPoint);
	//inline void translate(float x, float y, float z) { translate(Point3D(x, y, z)); }
	inline void move(float x, float y, float z) { translate(Point3D(Vector3D(x, y, z))); }
	void rotate(float phi, float theta, float eta);
	void scale(float x_factor, float y_factor, float z_factor);
	inline void scale(float factor) { scale(factor, factor, factor); }

protected:
	void transform(Matrix3D transformationMatrix);

private:
	Shape3D* m_shape3D;
};
