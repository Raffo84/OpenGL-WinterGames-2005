#include "Actor.h"

Actor::Actor(void) {
	pos = Vector3D(0.0f, 0.0f, 0.0f);
	rot = Vector3D(0.0f, 0.0f, 0.0f);
	radius = 1.0f;
}

Actor::Actor(Vector3D p, Vector3D rot, float r) {
	pos = p;
	rot = rot;
	radius = r;
}

Actor::~Actor(void) {
}

void __fastcall Actor::move(float x, float y, float z) {
	pos.x += x;
	pos.y += y;
	pos.z += z;
}

void __fastcall Actor::move(Vector3D diff) {
	pos += diff;
}

void Actor::rotate(float x, float y, float z) {
	rot.x += x;
	rot.y += y;
	rot.z += z;
	if(rot.x > 360.0f)
		rot.x -= 360.0f;
	if(rot.y > 360.0f)
		rot.y -= 360.0f;
	if(rot.z > 360.0f)
		rot.z -= 360.0f;
}

bool __fastcall Actor::operator &&(Actor* other) {
	if(other == this)
		return true;
	if(other == NULL)
		return false;
	Vector3D distVec = other->pos - pos;
	float dist = distVec.length();
	dist -= radius;
	dist -= other->radius;
	return (dist < 0.0f)?(true):(false);
}