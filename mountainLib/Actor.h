#pragma once

#include "../coolLib/Vector3D.h"
#include "../WinterGames/define.h"

class Actor
{
public:
	Actor(void);
	Actor(Vector3D pos, Vector3D rot, float r = 1.0f);
	virtual ~Actor(void);

	virtual int getType() const = 0;
	virtual void render() const = 0;
	void __fastcall move(float x, float y, float z);
	void __fastcall move(Vector3D diff);
	void rotate(float phi, float theta, float eta);

	Vector3D __fastcall getPosition() const { return pos; }
	Vector3D __fastcall getRotation() const { return rot; }
	float	 __fastcall getRadius()   const { return radius; }

	void setPosition(Vector3D newPos) { pos = newPos; }
	void setRotation(Vector3D newRot) { rot = newRot; }
	void setRadius(float r) { radius = r; }

	bool __fastcall operator&&(Actor* other);

protected:
	Vector3D	pos;
	Vector3D	rot;
	float		radius;
};
