#include "Carver.h"

Carver::Carver(void)
{
	lod = 4.0f;
	colSkis 	= Color(0.0f,0.13f, 0.5f);
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

Carver::Carver(const Avatar* rhs) {
	lod = rhs->getLOD();
	colSkis		= Color(0.0f,0.13f, 0.5f);
	colBoots	= rhs->colBoots;
	colTrousers	= rhs->colTrousers;
	colShirt	= rhs->colShirt;
	colGloves	= rhs->colGloves;
	colSkin		= rhs->colSkin;
	colHair		= rhs->colHair;

	movement = rhs->movement;
	rotation = rhs->rotation;

	setAllPoints(&Point3D(Vector3D(0.0f, 0.0f, 0.0f)), 1);
	flush();
}

Carver::~Carver(void)
{
	int idx;
	while(idx = (int)textureArray.size()) {
		delete textureArray[idx - 1];
		textureArray.pop_back();
	}

	glDeleteLists(dlIndex, 1);
	dlIndex = 0;
}

void Carver::generateDisplayList() {
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

		glScalef(AVATAR_SIZE / 2.9f, AVATAR_SIZE / 2.9f, AVATAR_SIZE / 2.9f);

		glDisable(GL_TEXTURE_2D);

		// Ski
		glDisable(GL_CULL_FACE);
		colSkis.switchToColor();
		glNormal3f(0.0f, 1.0f, 0.0f);
		// left
		glPushMatrix();
			glTranslatef(0.0f, 0.0f, 0.4f);
			glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(-2.5f, 0.15f, 0.0f);
			glVertex3f(-2.1f, 0.0f, 0.3f);
			glVertex3f(-2.1f, 0.0f, -0.3f);
			glVertex3f(0.0f, 0.0f, 0.2f);
			glVertex3f(0.0f, 0.0f, -0.2f);
			glVertex3f(2.5f, 0.0f, 0.3f);
			glVertex3f(2.5f, 0.0f, -0.3f);
			glEnd();
		glPopMatrix();
		// right
		glPushMatrix();
			glTranslatef(0.0f, 0.0f, -0.4f);
			glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(-2.5f, 0.15f, 0.0f);
			glVertex3f(-2.1f, 0.0f, 0.3f);
			glVertex3f(-2.1f, 0.0f, -0.3f);
			glVertex3f(0.0f, 0.0f, 0.2f);
			glVertex3f(0.0f, 0.0f, -0.2f);
			glVertex3f(2.5f, 0.0f, 0.3f);
			glVertex3f(2.5f, 0.0f, -0.3f);
			glEnd();
		glPopMatrix();
		if(isEnBFC) glEnable(GL_CULL_FACE);

		// Carver
		// -------
		// Legs
		glPushMatrix();		// right
			glTranslatef(0.0f, 0.0f, -0.4f);
			colBoots.switchToColor();
			glPushMatrix();
				glTranslatef(-0.09f, 0.11f, 0.0f);
				glScalef(1.5f, 0.5f, 1.1f);
				glutSolidCube(0.4f);
			glPopMatrix();
			colTrousers.switchToColor();
			glTranslatef(-0.09f, 0.45f, 0.0f);
			glRotatef(15.0f, 0.0f, 0.0f, 1.0f);
			glScalef(0.2f, 0.8f, 0.2f);
			renderHelpCylinder();
			glScalef(5.0f, 1.25f, 5.0f);

			glTranslatef(0.0f, 0.27f, 0.0f);
			glRotatef(-55.0f, 0.0f, 0.0f, 1.0f);
			glRotatef(10.0f, 1.0f, 0.0f, 0.0f);
			glTranslatef(0.0f, 0.29f, 0.0f);
			glScalef(0.2f, 0.8f, 0.2f);
			renderHelpCylinder();
		glPopMatrix();
		glPushMatrix();		// left
			glTranslatef(0.0f, 0.0f, 0.4f);
			colBoots.switchToColor();
			glPushMatrix();
				glTranslatef(-0.09f, 0.11f, 0.0f);
				glScalef(1.5f, 0.5f, 1.1f);
				glutSolidCube(0.4f);
			glPopMatrix();
			colTrousers.switchToColor();
			glTranslatef(-0.09f, 0.45f, 0.0f);
			glRotatef(15.0f, 0.0f, 0.0f, 1.0f);
			glScalef(0.2f, 0.8f, 0.2f);
			renderHelpCylinder();
			glScalef(5.0f, 1.25f, 5.0f);

			glTranslatef(0.0f, 0.27f, 0.0f);
			glRotatef(-55.0f, 0.0f, 0.0f, 1.0f);
			glRotatef(-10.0f, 1.0f, 0.0f, 0.0f);
			glTranslatef(0.0f, 0.29f, 0.0f);
			glScalef(0.2f, 0.8f, 0.2f);
			renderHelpCylinder();
		glPopMatrix();

		// Body
		glTranslatef(0.2f, 1.3f, 0.0f);
		glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
		glRotatef(25.0f, 1.0f, 0.0f, 0.0f);
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
			glTranslatef(-0.5f, -0.2f, 0.3f);
			glRotatef(95.0f, 0.0f, 0.0f, 1.0f);
			glRotatef(65.0f, 1.0f, 0.0f, 0.0f);
			glPushMatrix();
			glScalef(0.15f, 0.7f, 0.15f);
			renderHelpCylinder();
			glPopMatrix();

			glTranslatef(0.0f, 0.3f, 0.0f);
			//glRotatef(20.0f, 0.0f, 0.0f, 1.0f);
			glRotatef(15.0f, 1.0f, 0.0f, 0.0f);
			glTranslatef(0.0f, 0.3f, 0.0f);
			glPushMatrix();
			glScalef(0.15f, 0.7f, 0.15f);
			renderHelpCylinder();
			glPopMatrix();

			// hand
			colGloves.switchToColor();
			glTranslatef(0.0f, 0.35f, 0.0f);
			glutSolidSphere(0.2, (int)lod, (int)lod);
			// ski pole
			colSkis.switchToColor();
			glRotatef(-15.0f, 0.0f, 1.0f, 0.0f);
			glBegin(GL_LINES);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(-1.4f, 0.0f, 0.0f);
			glEnd();
			glTranslatef(-1.25f, 0.0f, 0.0f);
			glDisable(GL_CULL_FACE);
			glBegin(GL_TRIANGLE_FAN);
			glNormal3f(1.25f, 0.0f, 0.0f);
			for(int i = (int)lod; i >= 0; i--) {
				float y, z;
				y = 0.17f * cos(i * (2.0f*PI / lod));
				z = 0.17f * sin(i * (2.0f*PI / lod));
				glVertex3f(0.0f, y, z);
			}
			glEnd();
			if(isEnBFC) glEnable(GL_CULL_FACE);
		glPopMatrix();
		glPushMatrix();
			// left
			colShirt.switchToColor();
			glTranslatef(0.5f, -0.2f, 0.3f);
			glRotatef(95.0f, 0.0f, 0.0f, 1.0f);
			glRotatef(-65.0f, 1.0f, 0.0f, 0.0f);
			glPushMatrix();
			glScalef(0.15f, 0.7f, 0.15f);
			renderHelpCylinder();
			glPopMatrix();

			glTranslatef(0.0f, -0.3f, 0.0f);
			//glRotatef(-20.0f, 0.0f, 0.0f, 1.0f);
			glRotatef(-15.0f, 1.0f, 0.0f, 0.0f);
			glTranslatef(0.0f, -0.3f, 0.0f);
			glPushMatrix();
			glScalef(0.15f, 0.7f, 0.15f);
			renderHelpCylinder();
			glPopMatrix();

			// hand
			colGloves.switchToColor();
			glTranslatef(0.0f, -0.35f, 0.0f);
			glutSolidSphere(0.2, (int)lod, (int)lod);
			// ski pole
			colSkis.switchToColor();
			glRotatef(-5.0f, 0.0f, 1.0f, 0.0f);
			glBegin(GL_LINES);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(-1.4f, 0.0f, 0.0f);
			glEnd();
			glTranslatef(-1.25f, 0.0f, 0.0f);
			glDisable(GL_CULL_FACE);
			glBegin(GL_TRIANGLE_FAN);
			glNormal3f(1.25f, 0.0f, 0.0f);
			for(int i = (int)lod; i >= 0; i--) {
				float y, z;
				y = 0.17f * cos(i * (2.0f*PI / lod));
				z = 0.17f * sin(i * (2.0f*PI / lod));
				glVertex3f(0.0f, y, z);
			}
			glEnd();
			if(isEnBFC) glEnable(GL_CULL_FACE);
		glPopMatrix();

		// Head
		colSkin.switchToColor();
		glPushMatrix();	// neck
			glTranslatef(0.0f, 0.05f, 0.0f);
			glScalef(0.15f, 0.2f, 0.15f);
			renderHelpCylinder();
		glPopMatrix();
		glTranslatef(0.0f, 0.35f, 0.0f);
		glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		glRotatef(-10.0f, 0.0f, 0.0f, 1.0f);
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


