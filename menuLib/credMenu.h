#pragma once
#include "menu.h"
#include "../WinterGames/define.h"
#include "../coolTex/Bitmap.h"

#ifdef _DEBUG
#define CRED_PIC_FILE	"../textures/credits.bmp"
#else
#define CRED_PIC_FILE	"textures/credits.bmp"
#endif

class credMenu :
	public menu
{
public:
	credMenu(void);
	credMenu(menu* parent);
	~credMenu(void);

	void render() const;

private:
	void loadTexture();

	CAMP::Bitmap*	creditsPic;
};
