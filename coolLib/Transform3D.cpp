#include "Transform3D.h"
#include <iostream>
//#include <cmath>
//#define PI 3.1415926535897932384626433f

Transform3D::Transform3D(void)
{
}

Transform3D::~Transform3D(void)
{
}

void Transform3D::setShape3D(Shape3D* newShape) {
	m_shape3D = newShape;
}

Point3D Transform3D::getBoundingBoxCenter() {
	Point3D min, max;
	Point3D point;

	m_shape3D->iterate();
	m_shape3D->getNextPoint(&point);
	min = max = point;

	// HACK: commented out since no one currently needs Transform3D
	//while(m_shape3D->getNextPoint(&point)) {
	//	if(point.getPosition().x > max.getPosition().x)
	//		max.getPosition().x = point.getPosition().x;
	//	if(point.getPosition().x < min.getPosition().x)
	//		min.getPosition().x = point.getPosition().x;

	//	if(point.getPosition().y > max.getPosition().y)
	//		max.getPosition().y = point.getPosition().y;
	//	if(point.getPosition().y < min.getPosition().y)
	//		min.getPosition().y = point.getPosition().y;

	//	if(point.getPosition().z > max.getPosition().z)
	//		max.getPosition().z = point.getPosition().z;
	//	if(point.getPosition().z < min.getPosition().z)
	//		min.getPosition().z = point.getPosition().z;
	//}

	point = (max - min) * 0.5f;
	point = min + point;
	return point;
}

void Transform3D::translate(Point3D transPoint) {
	Point3D point;
	m_shape3D->iterate();
	while (m_shape3D->getNextPoint(&point)) {
		point = point + transPoint;
		m_shape3D->setCurrentPoint(point);
	}
}

void Transform3D::rotate(float phi, float theta, float eta){
	Matrix3D* rotMat = new Matrix3D();
	rotMat->createRotationMatrix(phi, theta, eta);
	transform(*rotMat);
}

void Transform3D::scale(float x_factor, float y_factor, float z_factor){
	Matrix3D sclMat;
	sclMat.createScaleMatrix(x_factor, y_factor, z_factor);
	Point3D center;
	center = getBoundingBoxCenter();

	translate((center * -1));
	transform(sclMat);
	translate(center);
}

void Transform3D::transform(Matrix3D transMat) {
	Point3D point;
	m_shape3D->iterate();
	while(m_shape3D->getNextPoint(&point)) {
		//std::cout << point.x << "\t";
		point = transMat * point.getPosition();
		//std::cout << point.x << std::endl;
		m_shape3D->setCurrentPoint(point);
	}
}