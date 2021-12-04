#include "Avatar.h"

Avatar::Avatar(void)
{
	possibleHairColors[0] = Color(0.8f, 0.4f, 0.0f);
	possibleHairColors[1] = Color(0.4f, 0.2f, 0.0f);
	possibleHairColors[2] = Color(0.0f, 0.0f, 0.0f);

	possibleSkinColors[0] = Color(1.0f, 0.6f, 0.4f);
	possibleSkinColors[1] = Color(0.8f, 0.4f, 0.2f);
	possibleSkinColors[2] = Color(0.4f, 0.2f, 0.1f);
	possibleSkinColors[3] = Color(1.0f, 0.8f, 0.2f);

	dlIndex = 0;
	//srand((unsigned int)time(NULL));
}

Avatar::~Avatar(void)
{
	unsigned int idx;
	while(idx = (unsigned int)textureArray.size()) {
		delete textureArray[idx - 1];
		textureArray.pop_back();
	}

	glDeleteLists(dlIndex, 1);
	dlIndex = 0;
}

void Avatar::render() const
{
	glPushMatrix();
	glTranslatef(movement.x, movement.y, movement.z);
	glRotatef(rotation.x, 1.0f, 0.0f, 0.0f);
	glRotatef(rotation.y, 0.0f, 1.0f, 0.0f);
	glRotatef(rotation.z, 0.0f, 0.0f, 1.0f);

	if(glIsList(dlIndex))
		glCallList(dlIndex);

	glPopMatrix();
}

Vector3D* Avatar::getCenter() const {
	Vector3D* vec = new Vector3D(&m_points[0].getPosition());
	return vec;
}

void Avatar::setLOD(float newLOD) {
	if(newLOD > 3 && newLOD != lod) {
		lod = newLOD;
		flush();
	}
}

void Avatar::randomizeColors(int flags) {
	float r, g, b;
	int idx;

	if(flags & AVATAR_COLOR_BOOTS) {
		r = (float)rand() / (float)RAND_MAX;
		g = (float)rand() / (float)RAND_MAX;
		b = (float)rand() / (float)RAND_MAX;
		colBoots = Color(r, g, b);
	}

	if(flags & AVATAR_COLOR_GLOVES) {
		r = (float)rand() / (float)RAND_MAX;
		g = (float)rand() / (float)RAND_MAX;
		b = (float)rand() / (float)RAND_MAX;
		colGloves = Color(r, g, b);
	}

	if(flags & AVATAR_COLOR_SHIRT) {
		r = (float)rand() / (float)RAND_MAX;
		g = (float)rand() / (float)RAND_MAX;
		b = (float)rand() / (float)RAND_MAX;
		colShirt = Color(r, g, b);
	}

	if(flags & AVATAR_COLOR_TROUSERS) {
		r = (float)rand() / (float)RAND_MAX;
		g = (float)rand() / (float)RAND_MAX;
		b = (float)rand() / (float)RAND_MAX;
		colTrousers = Color(r, g, b);
	}

	if(flags & AVATAR_COLOR_SKIN) {
		idx = rand() % AVATAR_MAX_NUM_SKIN_COL;
		while(colSkin == possibleSkinColors[idx])
			idx = rand() % AVATAR_MAX_NUM_SKIN_COL;
		colSkin = possibleSkinColors[idx];
	}

	if(flags & AVATAR_COLOR_HAIR) {
		idx = rand() % AVATAR_MAX_NUM_HAIR_COL;
		while(colHair == possibleHairColors[idx])
			idx = rand() % AVATAR_MAX_NUM_HAIR_COL;
		colHair = possibleHairColors[idx];
	}

	flush();
}

Color Avatar::getColor(int index) const {
	switch(index) {
	case AVATAR_COLOR_HAIR:
		return colHair;
	case AVATAR_COLOR_SKIN:
		return colSkin;
	case AVATAR_COLOR_SHIRT:
		return colShirt;
	case AVATAR_COLOR_GLOVES:
		return colGloves;
	case AVATAR_COLOR_TROUSERS:
		return colTrousers;
	case AVATAR_COLOR_BOOTS:
		return colBoots;
	default:
#ifdef _DEBUG
		std::cerr << "Avatar::getColor(index = " << index << "): Fehler, falscher Index!\n";
#endif
		return NULL;
	}
}

void Avatar::setColor(Color col, int flags) {
	if(flags & AVATAR_COLOR_HAIR)
		colHair = col;
	if(flags & AVATAR_COLOR_SKIN)
		colSkin = col;
	if(flags & AVATAR_COLOR_SHIRT)
		colShirt = col;
	if(flags & AVATAR_COLOR_GLOVES)
		colGloves = col;
	if(flags & AVATAR_COLOR_TROUSERS)
		colTrousers = col;
	if(flags & AVATAR_COLOR_BOOTS)
		colBoots = col;

	flush();
}

void Avatar::renderHelpCylinder() const {
	glBegin(GL_QUAD_STRIP);
	for(int i = 0; i <= lod; i++) {	// lod + 1 Durchgänge, um den Zylinder zu schließen.
		float x, z;
		x = cos(i * (2.0f*PI / lod));
		z = sin(i * (2.0f*PI / lod));
		glNormal3f(0.5f*x,  0.0f, 0.5f*z);
		glVertex3f(x, -0.5f, z);
		glVertex3f(x,  0.5f, z);
	}
	glEnd();
}

void Avatar::renderHelpSunglasses() const {
	//GLfloat matSpecFull[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//GLfloat matSpecNone[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	//// Make the Sunglasses all shiny
	//glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecFull);
	//glMaterialf (GL_FRONT, GL_SHININESS, 5.0f);

	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
	glVertex3f(cos(85.0f / 180.0f*PI)*0.3f, -0.1f, -sin(85.0f / 180.0f*PI)*0.3f);
	glVertex3f(0.0f, 0.0f, -0.31f);
	glVertex3f(-0.05f-cos(45.0f / 180.0f*PI)*0.3f, 0.05f, -0.05f -sin(45.0f / 180.0f*PI)*0.3f);
	glVertex3f(-0.31f, 0.05f, 0.0f);		// Center of Glasses
	glVertex3f(-0.05f-cos(45.0f / 180.0f*PI)*0.3f, 0.05f, 0.05f +sin(45.0f / 180.0f*PI)*0.3f);
	glVertex3f(0.0f, 0.0f, 0.31f);
	glVertex3f(cos(85.0f / 180.0f*PI)*0.3f, -0.1f, sin(85.0f / 180.0f*PI)*0.3f);
	glEnd();
	glBegin(GL_QUADS);
	glNormal3f(-0.05f-cos(45.0f / 180.0f*PI)*0.3f, 0.0f, -0.05f -sin(45.0f / 180.0f*PI)*0.3f);
	glVertex3f(-0.05f-cos(45.0f / 180.0f*PI)*0.3f, 0.05f, -0.05f -sin(45.0f / 180.0f*PI)*0.3f);
	glVertex3f(-0.3f, -0.05f, -0.174755f);
	glVertex3f(-0.3f, -0.05f, -0.087377f);
	glVertex3f(-0.31f, 0.05f, 0.0f);		// Center of Glasses

	glNormal3f(-0.05f-cos(45.0f / 180.0f*PI)*0.3f, 0.0f, 0.05f +sin(45.0f / 180.0f*PI)*0.3f);
	glVertex3f(-0.31f, 0.05f, 0.0f);		// Center of Glasses
	glVertex3f(-0.3f, -0.05f, 0.087377f);	// -0.262132f
	glVertex3f(-0.3f, -0.05f, 0.174755f);	// -0.214264f
	glVertex3f(-0.05f-cos(45.0f / 180.0f*PI)*0.3f, 0.05f, 0.05f +sin(45.0f / 180.0f*PI)*0.3f);
	glEnd();
	glPopMatrix();

	//glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecNone);
	//glMaterialf (GL_FRONT, GL_SHININESS, 0.0f);
}

bool Avatar::loadTexture(const char *fileName, int index) {
	if((unsigned int)index > textureArray.size() || index < 0) {
		// add new item
		CAMP::Bitmap* temp = new CAMP::Bitmap();
		temp->load(fileName);
		textureArray.push_back(temp);
	}
	else {
		// overwrite old item
		textureArray[index]->load(fileName);
	}
	return true;
}

void Avatar::activateTexture(int idx) const {
	if((unsigned int)idx < textureArray.size()) {
		int texWidth, texHeight;
		texWidth  = textureArray[idx]->getWidth();
		texHeight = textureArray[idx]->getHeight();
		glTexImage2D(GL_TEXTURE_2D, 0, 3, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureArray[idx]->getData());
	}
}

void Avatar::flush() {
	glDeleteLists(dlIndex, 1);
	dlIndex = 0;
	generateDisplayList();
}