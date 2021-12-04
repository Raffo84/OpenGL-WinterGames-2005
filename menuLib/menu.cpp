#include "menu.h"

menu::menu(void) {
	setParent(NULL);
	myTitle.clear();
	myCallbackFunc = NULL;
	activeIndex = 0;
	msgText.clear();
}

menu::menu(menu* father) {
	setParent(father);
	myTitle.clear();
	myCallbackFunc = NULL;
	activeIndex = 0;
	msgText.clear();
}

menu::~menu(void) {
	for(unsigned int i = 0; i < myEntries.size(); i++) {
		delete myEntries[i];
	}
	for(unsigned int i = 0; i < mySubmenus.size(); i++) {
		delete mySubmenus[i];
	}
}

void menu::addEntry(const std::string label) {
	menuEntry* newEntry = new menuEntry(label, (unsigned int)myEntries.size(), this);
	myEntries.push_back(newEntry);
}

void menu::addSubMenu(const std::string label, menu* submenu) {
	menuEntry* newEntry = new menuEntry(label, (unsigned int)myEntries.size(), this, (unsigned int)mySubmenus.size());
	myEntries.push_back(newEntry);
	mySubmenus.push_back(submenu);
	if(submenu->getTitle().empty())
		submenu->setTitle(label);
	submenu->hide();
}

void menu::setParent(menu* p) {
	myParent = p;
}

void menu::render() const {
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
			if(i == (unsigned int)activeIndex)
				activeColor.switchToColor();
			else
				fontColor.switchToColor();
			myEntries[i]->render(i);
		}
		glPopMatrix();

		// Messages
		if(!msgText.empty()) {
			unsigned int idx = (unsigned int)myEntries.size();
			menuEntry message = menuEntry(msgText, idx, NULL);
			glPushMatrix();
			glLoadIdentity();
			glTranslatef(MENU_COORD_X, MENU_COORD_Y - (2.0f * MENU_DISTANCE), MENU_COORD_Z);
			glTranslatef(MENU_DISTANCE, (MENU_DISTANCE / -2.0f), 0.0f);
			titleColor.switchToColor();
			message.render(idx);
			glPopMatrix();
		}

		if(isEnTex) glEnable(GL_TEXTURE_2D);
		glDisable(GL_NORMALIZE);
	}
}

void menu::show() {
	if(!isVis) {
		if(myParent)
			myParent->hide();	// re-hides us, too
		isVis = true;
		//activeIndex = 0;
	}
}

void menu::hide() {
	if(isVis)
		isVis = false;
	for(unsigned int i = 0; i < mySubmenus.size(); i++)
		mySubmenus[i]->hide();
	msgText.clear();
}

int menu::displayMessage(std::string newText) {
	msgText = newText;
	return MSG_NO_ERROR;
}

void menu::reset() {
	activeIndex = 0;
	for(unsigned int i = 0; i < mySubmenus.size(); i++)
		mySubmenus[i]->reset();
	msgText.clear();
}

bool menu::isVisible() const {
	if(isVis)
		return true;

	// check, if one of the submenus is visible
	for(unsigned int i = 0; i < mySubmenus.size(); i++) {
		if(mySubmenus[i]->isVisible())
			return true;
	}
	return false;
}

void menu::dispatchCallback(unsigned int index) const {
	if(index >= myEntries.size())
		return;
	menuEntry* current = myEntries[index];
	if(current->isSubmenu())
		mySubmenus[current->getSubmenuIndex()]->show();
	if(myCallbackFunc)
		(*myCallbackFunc)(const_cast<menu*>(this), index);
}

void menu::callbackFunc(void (*func)(menu*, unsigned int)) {
	myCallbackFunc = func;
}

menu* menu::getCurrentMenu() {
	if(isVis)
		return this;

	// check, if one of the submenus is visible
	for(unsigned int i = 0; i < mySubmenus.size(); i++) {
		if(mySubmenus[i]->isVisible()) {
			menu* current = mySubmenus[i]->getCurrentMenu();
			return current;
		}
	}
	return NULL;
}

menuEntry* menu::getEntry(int index) {
#pragma warning (disable: 4018)		// disable signed/unsigned warning
	if(index < myEntries.size())
		return myEntries[index];
	else
		return NULL;
#pragma warning (default: 4018)
}

void menu::setColors(Color font, Color selection, Color title) {
	fontColor = font;
	activeColor = selection;
	titleColor = title;
	for(unsigned int i = 0; i < mySubmenus.size(); i++) {
		mySubmenus[i]->setColors(font, selection, title);
	}
}

void menu::changeSelection(int offset) {
	if(isVis) {
		int max = (int)myEntries.size();
		activeIndex += offset;
		while(activeIndex < 0) {
			activeIndex += max;
		}
		while(activeIndex >= max) {
			activeIndex -= max;
		}
		if(!msgText.empty())
			msgText.clear();
	}
	else {
		for(unsigned int i = 0; i < mySubmenus.size(); i++) {
			if(mySubmenus[i]->isVisible()) {
				mySubmenus[i]->changeSelection(offset);
				return;
			}
		}
	}
}

void menu::fire() const {
	// check, which (sub)menu was meant
	if(isVis) {
		// it's us
		if(msgText.empty())
			dispatchCallback((unsigned int)activeIndex);
		else
			const_cast<std::string*>(&msgText)->clear();
		return;
	}

	// pass it to our submenus
	for(unsigned int i = 0; i < mySubmenus.size(); i++) {
		if(mySubmenus[i]->isVisible()) {
			mySubmenus[i]->fire();
			return;
		}
	}
}

void menu::operator=(const menu* rhs) {
	for(unsigned int i = 0; i < myEntries.size(); i++) {
		delete myEntries[i];
	}
	for(unsigned int i = 0; i < mySubmenus.size(); i++) {
		delete mySubmenus[i];
	}
	myTitle.clear();

	myTitle = rhs->myTitle;
	myCallbackFunc = rhs->myCallbackFunc;
	for(unsigned int i = 0; i < rhs->myEntries.size(); i++) {
		myEntries.push_back(rhs->myEntries[i]);
	}
	for(unsigned int i = 0; i < rhs->mySubmenus.size(); i++) {
		mySubmenus.push_back(rhs->mySubmenus[i]);
	}
}
