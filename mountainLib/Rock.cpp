#include "Rock.h"

unsigned int	Rock::totalNumber = 0;
unsigned int	Rock::myDisplayList = 0;
CAMP::Bitmap*	Rock::myTex = NULL;

Rock::Rock(void) {
	if(totalNumber == 0) {
		createMyDL();
#ifdef _DEBUG
		std::clog << "erster Fels, erstelle DL\n";
#endif
	}
	totalNumber++;
	radius = 1.0f;
	float angle = ((float)rand() / (float)RAND_MAX) * 360.0f;
	rotate(0.0f, angle, 0.0f);
}

Rock::~Rock(void) {
	totalNumber--;
	if(totalNumber == 0) {
		glDeleteLists(myDisplayList, 1);
		myDisplayList = 0;
		delete myTex;
#ifdef _DEBUG
		std::clog << "letzter Fels geloescht, entferne DL\n";
#endif
	}
}

void Rock::render() const {
	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z);
	glRotatef(rot.x, 1.0f, 0.0f, 0.0f);
	glRotatef(rot.y, 0.0f, 1.0f, 0.0f);
	glRotatef(rot.z, 0.0f, 0.0f, 1.0f);

	if(glIsList(myDisplayList))
		glCallList(myDisplayList);

#ifdef _DEBUG
	std::string text = "Rock";
	glColor3f(1.0f, 0.0f, 0.0f);
	glRasterPos3f(-0.5f, 2.0f, 0.0f);
	for(unsigned int i = 0; i < text.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, (int)text[i]);
	}
#endif

	glPopMatrix();
}

void Rock::createMyDL() {
	if(glIsList(myDisplayList))
		glDeleteLists(myDisplayList, 1);
	else {
		myDisplayList = glGenLists(1);
		if(myDisplayList == 0) {
#ifdef _DEBUG
			std::cerr << "\n\t!!!!!\nKrass, keine DL's mehr frei?!?\n\t!!!!!\n\n";
#endif
			return;
		}
	}

	myTex = new CAMP::Bitmap();
	myTex->load(TEXTURE_ROCK_FILE);
	glNewList(myDisplayList, GL_COMPILE);
	{
		GLboolean isEnTex = glIsEnabled(GL_TEXTURE_2D);
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_NORMALIZE);
		glPushMatrix();
		{
			float lod = GLOBAL_LEVEL_OF_DETAIL;
			glColor3f(0.8f, 0.8f, 0.8f);	// 0.6f
			glTexImage2D(GL_TEXTURE_2D, 0, 3,
				myTex->getWidth(), myTex->getHeight(),
				0, GL_RGB, GL_UNSIGNED_BYTE,
				myTex->getData());
			glEnable(GL_TEXTURE_2D);
			glScalef(3.0f, 3.0f, 3.0f);

			// Top
			glBegin(GL_TRIANGLE_STRIP);
			/* Just a little ASCII sketch
				(as seen from top)

			          ___,...---~~6~~--..__8___
			    _,--4´           / \       |\  `--..__
			  2´    |\          /   \      | \        `-10
			 / \    / \         |    |    /   \        / |
			|   |   |  \       /     \    |    |      /  |
			1   |  /   |      /       \   |    \     /   11
			`.  \  |    \     |        |  |     \   /   /
			  `. |/      \   /         \  |      \ /  ,´
			    `3..__    \ /           \/      _,9--´
			          `~~--5---..___..---7---~~´

			*/
			glNormal3f(-0.4f, 0.4f, 0.0f);		// 1
			glTexCoord2f(0.5f, 0.0f);
			glVertex3f(-0.5f,  0.3f,   0.0f);
			glNormal3f(-0.05f, 0.5f, 0.2f);		// 2
			glTexCoord2f(0.7f, 0.08f);
			glVertex3f(-0.4f,  0.5f,   0.2f);
			glNormal3f(0.0f, 0.5f, 0.0f);		// 3
			glTexCoord2f(0.1f, 0.12f);
			glVertex3f(-0.39f, 0.45f, -0.2f);
			glNormal3f(0.3f, 0.4f, -0.1f);		// 4
			glTexCoord2f(0.8f, 0.4f);
			glVertex3f(-0.3f,  0.5f,   0.2f);
			glNormal3f(0.0f, 0.5f, -0.1f);		// 5
			glTexCoord2f(0.05f, 0.6f);
			glVertex3f(-0.12f, 0.48f, -0.3f);
			glNormal3f(-0.2f, 0.4f, -0.2f);		// 6
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f( 0.0f,  0.46f,  0.31f);
			glNormal3f(0.2f, 0.4f, -0.1f);		// 7
			glTexCoord2f(0.0f, 1.3f);
			glVertex3f( 0.18f, 0.42f, -0.29f);
			glNormal3f(0.1f, 0.45f, 0.1f);		// 8
			glTexCoord2f(1.0f, 1.35f);
			glVertex3f( 0.21f, 0.49f,  0.29f);
			glNormal3f(-0.2f, 0.5f, -0.1f);		// 9
			glTexCoord2f(0.15f, 1.4f);
			glVertex3f( 0.35f, 0.38f, -0.18f);
			glNormal3f(0.3f, 0.4f, 0.2f);		// 10
			glTexCoord2f(0.6f, 2.0f);
			glVertex3f( 0.48f, 0.3f,   0.1f);
			glNormal3f(0.4f, 0.3f, 0.0f);		// 11
			glTexCoord2f(0.4f, 2.0f);
			glVertex3f( 0.5f,  0.32f, -0.1f);
			glEnd();

			// Sides
			glBegin(GL_TRIANGLE_STRIP);
			glNormal3f(-0.5f, 0.0f, 0.0f);			// 1
			glTexCoord2f(0.8f, 2.0f);
			glVertex3f(-0.5f,  0.3f,   0.0f);
			glTexCoord2f(-0.5f, 2.0f);
			glVertex3f(-0.46f, -1.0f,  0.1f);
			glNormal3f(-0.45f, 0.0f, 0.2f);			// 2
			glTexCoord2f(1.0f, 1.6f);
			glVertex3f(-0.4f,  0.5f,   0.2f);
			glTexCoord2f(-0.5f, 1.6f);
			glVertex3f(-0.3f,  -1.0f,  0.21f);
			glNormal3f(-0.1f, 0.0f, 0.5f);			// 4
			glTexCoord2f(1.0f, 1.2f);
			glVertex3f(-0.3f,  0.5f,   0.2f);
			glTexCoord2f(-0.5f, 1.2f);
			glVertex3f(-0.21f, -1.0f,  0.21f);
			glNormal3f(0.0f, 0.0f, 0.5f);			// 6
			glTexCoord2f(1.0f, 0.8f);
			glVertex3f( 0.0f,  0.46f,  0.31f);
			glTexCoord2f(-0.5f, 0.8f);
			glVertex3f(-0.05f, -1.0f,  0.29f);
			glNormal3f(0.2f, 0.0f, 0.4f);			// 8
			glTexCoord2f(1.0f, 0.4f);
			glVertex3f( 0.21f, 0.49f,  0.29f);
			glTexCoord2f(-0.5f, 0.4f);
			glVertex3f( 0.23f, -1.0f,  0.26f);
			glNormal3f(0.5f, 0.0f, 0.1f);			// 10
			glTexCoord2f(0.8f, 0.0f);
			glVertex3f( 0.48f, 0.3f,   0.1f);
			glTexCoord2f(-0.5f, 0.0f);
			glVertex3f( 0.44f, -1.0f,  0.03f);

			glNormal3f(0.5f, 0.0f, -0.1f);			// 11
			glTexCoord2f(0.8f, 2.0f);
			glVertex3f( 0.5f,  0.32f, -0.1f);
			glTexCoord2f(-0.5f, 2.0f);
			glVertex3f( 0.5f,  -1.0f, -0.1f);
			glNormal3f(0.35f, 0.0f, -0.35f);		// 9
			glTexCoord2f(0.9f, 1.6f);
			glVertex3f( 0.35f, 0.38f, -0.18f);
			glTexCoord2f(-0.5f, 1.6f);
			glVertex3f( 0.21f, -1.0f, -0.24f);
			glNormal3f(0.2f, 0.0f, -0.45f);			// 7
			glTexCoord2f(1.0f, 1.2f);
			glVertex3f( 0.18f, 0.42f, -0.29f);
			glTexCoord2f(-0.5f, 1.2f);
			glVertex3f( 0.05f, -1.0f, -0.3f);
			glNormal3f(-0.15f, 0.0f, -0.5f);		// 5
			glTexCoord2f(1.0f, 0.8f);
			glVertex3f(-0.12f, 0.48f, -0.3f);
			glTexCoord2f(-0.5f, 0.8f);
			glVertex3f(-0.25f, -1.0f, -0.25f);
			glNormal3f(-0.45f, 0.0f, -0.2f);		// 3
			glTexCoord2f(1.0f, 0.4f);
			glVertex3f(-0.39f, 0.45f, -0.2f);
			glTexCoord2f(-0.5f, 0.4f);
			glVertex3f(-0.44f, -1.0f, -0.13f);
			glNormal3f(-0.5f, 0.0f, 0.0f);			// 1
			glTexCoord2f(0.8f, 0.0f);
			glVertex3f(-0.5f,  0.3f,   0.0f);
			glTexCoord2f(-0.5f, 0.0f);
			glVertex3f(-0.46f, -1.0f,  0.1f);
			glEnd();
		}
		glPopMatrix();
		glDisable(GL_NORMALIZE);
		if(isEnTex) glEnable(GL_TEXTURE_2D);
	}
	glEndList();
}
