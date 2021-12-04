#include "lanplayer.h"

LANPlayer::LANPlayer(void) {
	nameCol = NULL;
}

LANPlayer::~LANPlayer(void) {
}

void LANPlayer::render() const {
	glPushMatrix();
	GLboolean isEnTex = glIsEnabled(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glTranslatef(pos.x, pos.y, pos.z);
	glRotatef(-camRot.x, 1.0f, 0.0f, 0.0f);
	glRotatef(-camRot.y, 0.0f, 1.0f, 0.0f);
	glRotatef(-camRot.z, 0.0f, 0.0f, 1.0f);

	unsigned int length = (unsigned int)name.length();

	glTranslatef(0.0f, 2.2f, 0.0f);
	glScalef(0.004f, 0.004f, 0.004f);
	glTranslatef((float)(glutStrokeWidth(GLUT_STROKE_ROMAN, (int)'n')) * (float)length / -2.0f, 0.0f, 0.0f);
	if(nameCol == NULL)
		glColor3f(PLAYER_NAME_COLOR_R, PLAYER_NAME_COLOR_G, PLAYER_NAME_COLOR_B);
	else
		nameCol->switchToColor();
	for(unsigned int i = 0; i < length; i++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)name[i]);

	glEnable(GL_LIGHTING);
	if(isEnTex)
		glEnable(GL_TEXTURE_2D);
	glPopMatrix();

	Player::render();
}