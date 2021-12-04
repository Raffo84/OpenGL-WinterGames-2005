#include "Boarder.h"

Boarder::Boarder(void)
{
	lod = 4.0f;
	colBoard	= _BOARDER_BOARD_COLOR;
	colBoots	= Color(0.2f, 0.2f, 0.0f);
	colTrousers	= Color(0.8f, 0.4f, 0.0f);
	colShirt	= Color(1.0f, 1.0f, 0.0f);
	colGloves	= Color(0.8f, 0.8f, 0.9f);
	colSkin		= Color(1.0f, 0.6f, 0.4f);
	colHair		= Color(0.4f, 0.2f, 0.0f);
	setAllPoints(&Point3D(Vector3D(0.0f, 0.0f, 0.0f)), 1);
	dlIndex = 0;
	generateDisplayList();
}

Boarder::Boarder(const Avatar* rhs) {
	lod = rhs->getLOD();
	colBoard	= _BOARDER_BOARD_COLOR;
	colBoots	= rhs->colBoots;
	colTrousers	= rhs->colTrousers;
	colShirt	= rhs->colShirt;
	colGloves	= rhs->colGloves;
	colSkin		= rhs->colSkin;
	colHair		= rhs->colHair;

	movement = rhs->getPosition();
	rotation = rhs->getRotation();

	setAllPoints(&Point3D(Vector3D(0.0f, 0.0f, 0.0f)), 1);
	flush();
}

Boarder::~Boarder(void)
{
	int idx;
	while(idx = (int)textureArray.size()) {
		delete textureArray[idx - 1];
		textureArray.pop_back();
	}

	glDeleteLists(dlIndex, 1);
	dlIndex = 0;
}

void Boarder::generateDisplayList() {
	if(glIsList(dlIndex))
		glDeleteLists(dlIndex, 1);
	else {
		dlIndex = glGenLists(1);
		if(dlIndex == 0) {
#ifdef _DEBUG
			std::cout << "\t!!!!!\nKrass, keine DL's mehr frei?!?\n";
#endif
			return;
		}
	}

	glNewList(dlIndex, GL_COMPILE);
		GLboolean isEnTex = glIsEnabled(GL_TEXTURE_2D);
		GLboolean isEnBFC = glIsEnabled(GL_CULL_FACE);
		glDisable(GL_TEXTURE_2D);

		glScalef(AVATAR_SIZE / 2.9f, AVATAR_SIZE / 2.9f, AVATAR_SIZE / 2.9f);

		//glPushMatrix();
		//glTranslatef(0.0f, 1.45f, 0.0f);
		//glScalef(5.0f, 2.9f, 1.0f);
		//glutSolidCube(1.0);
		//glPopMatrix();

		// Snowboard
		glDisable(GL_CULL_FACE);
		colBoard.switchToColor();
		activateTexture(0);
		glBegin(GL_QUADS);
		glNormal3f(0.0f, 2.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-2.0f, 0.0f, 0.5f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(2.0f, 0.0f, 0.5f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(2.0f, 0.0f, -0.5f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-2.0f, 0.0f, -0.5f);
		glEnd();
		activateTexture(1);
		glBegin(GL_TRIANGLE_FAN);	// back
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(2.0f, 0.0f, -0.5f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(2.0f, 0.0f, 0.5f);
		for(float i = 1; i <= lod; i++) {
			float x, z;
			x = 0.5f*sin(i * PI / (lod+1.0f));
			z = 0.5f*cos(i * PI / (lod+1.0f));
			glTexCoord2f(x, z);
			glVertex3f(2.0f + x, 0.0f, z);
		}		
		glEnd();
		glBegin(GL_TRIANGLE_FAN);	// front
		glVertex3f(-2.0f, 0.0f, 0.5f);
		glVertex3f(-2.0f, 0.0f, -0.5f);
		for(float i = lod; i >= 1; i--) {
			float x, z;
			x = 0.5f*sin(-i * PI / (lod+1.0f));
			z = 0.5f*cos(-i * PI / (lod+1.0f));
			glVertex3f(-2.0f + x, 0.0f, z);
		}		
		glEnd();
		if(isEnBFC) glEnable(GL_CULL_FACE);

		// Boarder
		// -------
		// Legs
		glPushMatrix();		// right
			glTranslatef(-0.7f, 0.0f, 0.0f);
			colBoots.switchToColor();
			glPushMatrix();
				glTranslatef(-0.05f, 0.11f, 0.1f);
				glScalef(1.1f, 0.5f, 1.5f);
				glutSolidCube(0.4f);
			glPopMatrix();
			colTrousers.switchToColor();
			glTranslatef(0.0f, 0.45f, 0.0f);
			glRotatef(-10.0f, 0.0f, 0.0f, 1.0f);
			glScalef(0.2f, 0.8f, 0.2f);
			renderHelpCylinder();
			glScalef(5.0f, 1.25f, 5.0f);

			glTranslatef(0.0f, 0.35f, 0.0f);
			glRotatef(-30.0f, 0.0f, 0.0f, 1.0f);
			glTranslatef(0.0f, 0.35f, 0.0f);
			glScalef(0.2f, 0.8f, 0.2f);
			renderHelpCylinder();
			glEnd();
		glPopMatrix();
		glPushMatrix();		// left
			glTranslatef(0.7f, 0.0f, 0.0f);
			colBoots.switchToColor();
			glPushMatrix();
				glTranslatef(0.05f, 0.11f, 0.1f);
				glScalef(1.1f, 0.5f, 1.5f);
				glutSolidCube(0.4f);
			glPopMatrix();
			colTrousers.switchToColor();
			glTranslatef(0.0f, 0.45f, 0.0f);
			glRotatef(10.0f, 0.0f, 0.0f, 1.0f);
			glScalef(0.2f, 0.8f, 0.2f);
			renderHelpCylinder();
			glScalef(5.0f, 1.25f, 5.0f);

			glTranslatef(0.0f, 0.35f, 0.0f);
			glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
			glTranslatef(0.0f, 0.35f, 0.0f);
			glScalef(0.2f, 0.8f, 0.2f);
			renderHelpCylinder();
		glPopMatrix();

		// Body
		glTranslatef(0.0f, 1.5f, 0.0f);
		glRotatef(10.0f, 1.0f, 0.0f, 0.0f);
		colTrousers.switchToColor();
		glPushMatrix();
			glScalef(0.5f, 0.5f, 0.25f);
			glBegin(GL_TRIANGLE_FAN);
			glNormal3f(0.0f, -0.5f, 0.0f);
			for(int i = 0; i <= lod; i++) {
				float x, z;
				x = cos(i * (2.0f*PI / lod));
				z = sin(i * (2.0f*PI / lod));
				glVertex3f(x, -0.5f, z);
			}
			glEnd();
			renderHelpCylinder();
		glPopMatrix();
		glTranslatef(0.0f, 0.5f, 0.0f);
		colShirt.switchToColor();
		glPushMatrix();
			glScalef(0.5f, 0.5f, 0.25f);
			glBegin(GL_TRIANGLE_FAN);
			glNormal3f(0.0f, 0.5f, 0.0f);
			for(int i = (int)lod; i >= 0; i--) {
				float x, z;
				x = cos(i * (2.0f*PI / lod));
				z = sin(i * (2.0f*PI / lod));
				glVertex3f(x, 0.5f, z);
			}
			glEnd();
			renderHelpCylinder();
		glPopMatrix();
		glTranslatef(0.0f, 0.25f, 0.0f);

		// Arms
		glPushMatrix();
			// right
			colShirt.switchToColor();
			glTranslatef(-0.65f, -0.2f, 0.05f);
			glRotatef(95.0f, 0.0f, 0.0f, 1.0f);
			glRotatef( 5.0f, 1.0f, 0.0f, 0.0f);
			glPushMatrix();
			glScalef(0.15f, 0.7f, 0.15f);
			renderHelpCylinder();
			glPopMatrix();

			glTranslatef(0.0f, 0.3f, 0.0f);
			glRotatef(20.0f, 0.0f, 0.0f, 1.0f);
			glRotatef(15.0f, 1.0f, 0.0f, 0.0f);
			glTranslatef(0.0f, 0.3f, 0.0f);
			glPushMatrix();
			glScalef(0.15f, 0.7f, 0.15f);
			renderHelpCylinder();
			glPopMatrix();

			// Hand
			colGloves.switchToColor();
			glTranslatef(0.0f, 0.35f, 0.0f);
			glutSolidSphere(0.2, (int)lod, (int)lod);
		glPopMatrix();
		glPushMatrix();
			// left
			colShirt.switchToColor();
			glTranslatef(0.65f, -0.2f, 0.05f);
			glRotatef(85.0f, 0.0f, 0.0f, 1.0f);
			glRotatef(-5.0f, 1.0f, 0.0f, 0.0f);
			glPushMatrix();
			glScalef(0.15f, 0.7f, 0.15f);
			renderHelpCylinder();
			glPopMatrix();

			glTranslatef(0.0f, -0.3f, 0.0f);
			glRotatef(-20.0f, 0.0f, 0.0f, 1.0f);
			glRotatef(-15.0f, 1.0f, 0.0f, 0.0f);
			glTranslatef(0.0f, -0.3f, 0.0f);
			glPushMatrix();
			glScalef(0.15f, 0.7f, 0.15f);
			renderHelpCylinder();
			glPopMatrix();

			// Hand
			colGloves.switchToColor();
			glTranslatef(0.0f, -0.35f, 0.0f);
			glutSolidSphere(0.2, (int)lod, (int)lod);
		glPopMatrix();

		// Head
		colSkin.switchToColor();
		glPushMatrix();	// neck
			glTranslatef(0.0f, 0.05f, 0.0f);
			glScalef(0.15f, 0.2f, 0.15f);
			renderHelpCylinder();
		glPopMatrix();
		glTranslatef(0.0f, 0.35f, 0.0f);
		glRotatef(40.0f, 0.0f, 1.0f, 0.0f);
		glutSolidSphere(0.3, (int)lod, (int)lod);
		glPushMatrix();	// Nose
			glTranslatef(-0.3f, -0.05f, 0.0f);
			glRotatef(10.0f, 1.0f, 0.0f, 0.0f);
			glRotatef(-100.0f, 1.0f, 0.0f, 0.0f);
			glutSolidCone(0.05, 0.1, (int)lod, (int)lod);
		glPopMatrix();
		glPushMatrix();	// Hair
			glTranslatef(0.0f, 0.3f, 0.0f);
			colHair.switchToColor();
			// TODO: Code für die Haare schreiben.
			glBegin(GL_LINE_STRIP);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.04f, 0.08f, 0.0f);
			glVertex3f(0.08f, 0.12f, 0.0f);
			glVertex3f(0.12f, 0.12f, 0.0f);
			glEnd();
		glPopMatrix();
		renderHelpSunglasses();

		if(isEnTex) glEnable(GL_TEXTURE_2D);
	glEndList();
}
