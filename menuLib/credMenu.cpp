#include "credmenu.h"

credMenu::credMenu(void)
	: menu() {
	loadTexture();
}

credMenu::credMenu(menu* father)
	: menu(father) {
	loadTexture();
}

credMenu::~credMenu(void) {
	delete creditsPic;
}

void credMenu::loadTexture() {
	creditsPic = new CAMP::Bitmap();
	creditsPic->load(CRED_PIC_FILE);
}

void credMenu::render() const {
	if(!isVis) {
		// check for visible children
		for(unsigned int i = 0; i < mySubmenus.size(); i++) {
			if(mySubmenus[i]->isVisible()) {
				mySubmenus[i]->render();
				break;
			}
		}
	}
	else {
		GLboolean isEnTex = glIsEnabled(GL_TEXTURE_2D);
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_NORMALIZE);

		// Title
		unsigned int len = (unsigned int)myTitle.length();
		glPushMatrix();
		titleColor.switchToColor();
		glLoadIdentity();
		glTranslatef((float)(glutStrokeWidth(GLUT_STROKE_ROMAN, (int)'n') * len) / -2000.0f, MENU_COORD_Y, MENU_COORD_Z);
		glScalef(0.001f, 0.001f, 0.001f);
		for(unsigned int i = 0; i <= len; i++) {
			glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)myTitle[i]);
		}
		glPopMatrix();

		// Entries
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(MENU_COORD_X, MENU_COORD_Y - (2.0f * MENU_DISTANCE), MENU_COORD_Z);
		for(unsigned int i = 0; i < myEntries.size(); i++) {
			if(myEntries[i]->getLabel()[0] != ' ')
				activeColor.switchToColor();
			else
				fontColor.switchToColor();
			myEntries[i]->render(i);
		}
		glPopMatrix();

		if(isEnTex) glEnable(GL_TEXTURE_2D);

		// Picture
		glPushMatrix();
		{
			float rotMat[16];
			glGetFloatv(GL_MODELVIEW_MATRIX, rotMat);
			glLoadIdentity();
			glTranslatef(1.0f, 0.0f, -4.0f);
			glMultMatrixf(rotMat);
			glRotatef(10.0f, 0.0f, 0.0f, 1.0f);

			glColor3f(1.0f, 1.0f, 1.0f);
			glTexImage2D(GL_TEXTURE_2D, 0, 3,
					creditsPic->getWidth(), creditsPic->getHeight(),
					0, GL_RGB, GL_UNSIGNED_BYTE,
					creditsPic->getData());

			// Front (Ralf)
			glBegin(GL_TRIANGLE_STRIP);
			glNormal3f(-1.0f, 1.0f, 1.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-1.0f,  1.0f, 0.0f);

			glNormal3f(-1.0f, -1.0f, 1.0f);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-1.0f, -1.0f, 0.0f);
			
			glNormal3f(1.0f, 1.0f, 1.0f);
			glTexCoord2f(0.5f, 0.0f);
			glVertex3f( 1.0f,  1.0f, 0.0f);
			
			glNormal3f(1.0f, -1.0f, 1.0f);
			glTexCoord2f(0.5f, 1.0f);
			glVertex3f( 1.0f, -1.0f, 0.0f);
			glEnd();

			// Back (Flo)
			glBegin(GL_TRIANGLE_STRIP);
			glNormal3f(-1.0f, 1.0f, -1.0f);
			glTexCoord2f(0.5f, 0.0f);
			glVertex3f( 1.0f,  1.0f, 0.0f);

			glNormal3f(-1.0f, -1.0f, -1.0f);
			glTexCoord2f(0.5f, 1.0f);
			glVertex3f( 1.0f, -1.0f, 0.0f);
			
			glNormal3f(1.0f, 1.0f, -1.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-1.0f,  1.0f, 0.0f);
			
			glNormal3f(1.0f, -1.0f, -1.0f);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(-1.0f, -1.0f, 0.0f);
			glEnd();
		}
		glPopMatrix();

		glDisable(GL_NORMALIZE);
	}
}