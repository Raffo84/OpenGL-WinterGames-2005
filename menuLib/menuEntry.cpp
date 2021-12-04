#include "menuEntry.h"

menuEntry::menuEntry(std::string lbl, unsigned int idx, menu* father) {
	setLabel(lbl);
	index = idx;
	parentMenu = father;

	isSub = false;
	subIndex = 0;
}

menuEntry::menuEntry(std::string lbl, unsigned int idx, menu* father, unsigned int subMenuIndex) {
	setLabel(lbl);
	index = idx;
	parentMenu = father;

	isSub = true;
	subIndex = subMenuIndex;
}

menuEntry::~menuEntry(void) {
}

void menuEntry::setLabel(std::string lbl) {
	label.clear();
	label = lbl;
}

void menuEntry::render(unsigned int pos) const {
	glPushMatrix();
	glTranslatef(0.0f, -(pos * MENU_DISTANCE), 0.0f);
	glScalef(0.001f, 0.001f, 0.001f);
	size_t len = label.length();
	for(unsigned int i = 0; i < len; i++) {
		//glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)label[i]);
		glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)label[i]);
	}
	glPopMatrix();
}
