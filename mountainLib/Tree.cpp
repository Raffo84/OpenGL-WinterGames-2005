#include "Tree.h"

unsigned int	Tree::totalNumber = 0;
unsigned int	Tree::myDisplayList = 0;
CAMP::Bitmap*	Tree::myTex = NULL;

Tree::Tree(void) {
	if(totalNumber == 0) {
		createMyDL();
#ifdef _DEBUG
		std::clog << "erster Tree, erstelle DL\n";
#endif
	}
	totalNumber++;
	radius = 1.0f;
	float angle = ((float)rand() / (float)RAND_MAX) * 360.0f;
	rotate(0.0f, angle, 0.0f);
}

Tree::~Tree(void) {
	totalNumber--;
	if(totalNumber == 0) {
		glDeleteLists(myDisplayList, 1);
		myDisplayList = 0;
		delete myTex;
#ifdef _DEBUG
		std::clog << "letzter Tree geloescht, entferne DL\n";
#endif
	}
}

void Tree::render() const {
	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z);
	glRotatef(rot.x, 1.0f, 0.0f, 0.0f);
	glRotatef(rot.y, 0.0f, 1.0f, 0.0f);
	glRotatef(rot.z, 0.0f, 0.0f, 1.0f);

	if(glIsList(myDisplayList))
		glCallList(myDisplayList);

#ifdef _DEBUG
	std::string text = "Tree";
	glColor3f(0.0f, 1.0f, 0.0f);
	glRasterPos3f(-0.5f, 3.0f, 0.0f);
	for(unsigned int i = 0; i < text.length(); i++) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, (int)text[i]);
	}
#endif

	glPopMatrix();
}

void Tree::createMyDL() {
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
	myTex->load(TREE_TEXTURE_FILE);
	glNewList(myDisplayList, GL_COMPILE);
	{
		GLboolean isEnTex = glIsEnabled(GL_TEXTURE_2D);
		glDisable(GL_TEXTURE_2D);
		glPushMatrix();
		{
			int lod = (int)GLOBAL_LEVEL_OF_DETAIL;
			glColor3f(0.4f, 0.0f, 0.0f);
			glTranslatef(0.0f, 0.5f, 0.0f);
			glBegin(GL_QUAD_STRIP);
			for(int i = 0; i <= lod; i++) {	// lod + 1 Durchgänge, um den Zylinder zu schließen.
				float x, z;
				x = 0.2f * cos(i * (2.0f*PI / lod));
				z = 0.2f * sin(i * (2.0f*PI / lod));
				glNormal3f(0.5f*x,  0.0f, 0.5f*z);
				glVertex3f(x, -1.0f, z);
				glVertex3f(x,  1.0f, z);
			}
			glEnd();
			glTranslatef(0.0f, 0.5f, 0.0f);
			glColor3f(0.0f, 1.0f, 0.0f);
			glTexImage2D(GL_TEXTURE_2D, 0, 3,
				myTex->getWidth(), myTex->getHeight(),
				0, GL_RGB, GL_UNSIGNED_BYTE,
				myTex->getData());
			glEnable(GL_TEXTURE_2D);

			//glutSolidCone(1.5f, 1.4f, lod, lod);
			renderHelp(1.5f, 1.4f, lod);
			//renderHelp(1.5f);

			glTranslatef(0.0f, 0.7f, 0.0f);
			renderHelp(1.2f, 1.4f, lod);
			//renderHelp(1.2f);

			glTranslatef(0.0f, 0.7f, 0.0f);
			renderHelp(0.9f, 1.4f, lod);
			//renderHelp(0.9f);

			glTranslatef(0.0f, 0.7f, 0.0f);
			renderHelp(0.6f, 1.0f, lod);
			//renderHelp(0.6f);

			glTranslatef(0.0f, 0.6f, 0.0f);
			renderHelp(0.3f, 0.7f, lod);
			//renderHelp(0.3f);
		}
		glPopMatrix();
		if(!isEnTex) glDisable(GL_TEXTURE_2D);
	}
	glEndList();
}

void Tree::renderHelp(float base, float height, int lod) {
	glEnable(GL_NORMALIZE);
	glPushMatrix();
	float angle = ((float)rand() / (float)RAND_MAX) * 360.0f;
	glRotatef(angle, 0.0f, 1.0f, 0.0f);

	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.5f, 0.5f);
	glVertex3f(0.0f, height, 0.0f);
    for(int i = 0; i <= lod; i++) {
		float x, z;
		x = cos(-i * (2.0f*PI / (float)lod));
		z = sin(-i * (2.0f*PI / (float)lod));
		glNormal3f(x, 0.5f, z);
		glTexCoord2f((0.5f * x) + 0.5f, (0.5f * z) + 0.5f);
		glVertex3f(base * x, 0.0f, base * z);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord2f(0.5f, 0.5f);
	glVertex3f(0.0f, 0.3f, 0.0f);
	for(int i = 0; i <= lod; i++) {
		float x, z;
		z = cos(-i * (2.0f*PI / lod));
		x = sin(-i * (2.0f*PI / lod));
		glTexCoord2f((0.5f * x) + 0.5f, (0.5f * z) + 0.5f);
		glVertex3f(base * x, 0.0f, base * z);
	}
	glEnd();

	glPopMatrix();
	glDisable(GL_NORMALIZE);
}

