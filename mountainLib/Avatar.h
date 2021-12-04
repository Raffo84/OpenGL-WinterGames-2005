#pragma once
#include "../coolLib/Shape3D.h"
#include "../coolLib/Color.h"
#include "../coolLib/Vector3D.h"
#include "../WinterGames/define.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

class Avatar :
	public Shape3D
{
	friend class Boarder;
	friend class Carver;

public:
	Avatar(void);
	virtual ~Avatar(void);

	Vector3D* getCenter() const;
	virtual void render() const;
	bool loadTexture(const char* fileName) { return loadTexture(fileName, -1); }
	bool loadTexture(const char* fileName, int index);
	void randomizeColors(int flags = -1);
	Color getColor(int index = 0) const;
	void  setColor(Color col, int flags);
	float getLOD() const { return lod; }
	void  setLOD(float newLOD);
	virtual int getType() const = 0;
	void flush();

protected:
	float lod;
	Color colBoots, colTrousers, colShirt, colGloves, colSkin, colHair;
	Color possibleSkinColors[AVATAR_MAX_NUM_SKIN_COL], possibleHairColors[AVATAR_MAX_NUM_HAIR_COL];
	vector<CAMP::Bitmap*> textureArray;

	void activateTexture() const { activateTexture(0); }
	void activateTexture(int index) const;
	void renderHelpCylinder() const;
	void renderHelpSunglasses() const;

	virtual void generateDisplayList() = 0;
};
