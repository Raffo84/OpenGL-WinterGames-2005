#include "lobbymenu.h"

lobbyMenu::lobbyMenu(void)
: menu() {
	init();
}

lobbyMenu::lobbyMenu(menu* father)
: menu(father) {
	init();
}

lobbyMenu::~lobbyMenu(void) {
	//delete ipAddressList;
}

void lobbyMenu::init() {
	gameFlags = GAME_STATUS_TYPE_FREE | GAME_STATUS_DAY;
	isClient = true;
	addEntry("Start ");
	addEntry("Beenden");
	//addEntry("Abbrechen");
	addEntry("Bot erstellen");
	addEntry("Spieltyp: Freestyle");
	addEntry("Uhrzeit: Tag  ");
	playerList = NULL;
	//ipAddressList = NULL;
}

void lobbyMenu::reset() {
	gameFlags = GAME_STATUS_TYPE_FREE | GAME_STATUS_DAY;
	isClient = true;
	//playerReadyFlags = 0;
	myEntries[3]->setLabel("Spieltyp: Freestyle");
	myEntries[4]->setLabel("Uhrzeit: Tag  ");
	//delete ipAddressList;
	//ipAddressList = NULL;
	ipAddressList.clear();
	menu::reset();
}

void lobbyMenu::setGameType(int newType) {
	if(gameFlags != newType) {
		gameFlags = newType;
		if(gameFlags & GAME_STATUS_TYPE_DOWN)
			myEntries[3]->setLabel("Spieltyp: Downhill ");
		else
			myEntries[3]->setLabel("Spieltyp: Freestyle");

		if(gameFlags & GAME_STATUS_NIGHT)
			myEntries[4]->setLabel("Uhrzeit: Nacht");
		else
			myEntries[4]->setLabel("Uhrzeit: Tag  ");
	}
}

void lobbyMenu::setIpList(std::vector<std::string> *newList) {
	//delete ipAddressList;
	//ipAddressList = newList;
	ipAddressList.clear();
	ipAddressList = *newList;
}

void lobbyMenu::setLobbyType(bool newFlag) {
	isClient = newFlag;
	if(isClient) {
		//delete ipAddressList;
		//ipAddressList = NULL;
		ipAddressList.clear();
		myEntries[0]->setLabel("Bereit");
		//myEntries[0]->setLabel("warten");
	}
	else
		myEntries[0]->setLabel("Start ");
}

int  lobbyMenu::getCurrentPlayer() const {
	if((unsigned int)activeIndex >= myEntries.size())
		return (activeIndex - (int)myEntries.size());
	else
		return -1;
}

void lobbyMenu::changeSelection(int offset) {
	if(isVis) {
		int max = (int)myEntries.size() + (int)playerList->size();
		activeIndex += offset;
		while(activeIndex < 0) {
			activeIndex += max;
		}
		while(activeIndex >= max) {
			activeIndex -= max;
		}
	}
	else
		menu::changeSelection(offset);
}

void lobbyMenu::show() {
	//myEntries[3]->setLabel("Spieltyp: Freestyle");
	//myEntries[4]->setLabel("Uhrzeit: Tag  ");
	//isClient = true;
	//gameFlags = GAME_STATUS_TYPE_FREE | GAME_STATUS_DAY;
	menu::show();
}

void lobbyMenu::render() const {
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

		// title
		unsigned int len = (unsigned int)myTitle.length();
		titleColor.switchToColor();
		glPushMatrix();
		glLoadIdentity();
		glTranslatef((float)(glutStrokeWidth(GLUT_STROKE_ROMAN, (int)'n') * len) / -2000.0f, MENU_COORD_Y, MENU_COORD_Z);
		glScalef(0.001f, 0.001f, 0.001f);
		for(unsigned int i = 0; i <= len; i++)
			glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)myTitle[i]);
		glPopMatrix();

		// control buttons
		{
			float buttonLength;
			float aveCharWidth = glutStrokeWidth(GLUT_STROKE_ROMAN, (int)'n') * 0.001f;
			glPushMatrix();
			glLoadIdentity();
			// assume 8 Players
			glTranslatef(MENU_COORD_X, MENU_COORD_Y - (10.5f * MENU_DISTANCE), MENU_COORD_Z);
			for(unsigned int but = 0; but < myEntries.size(); but++) {
				if(activeIndex == but)
					activeColor.switchToColor();
				else
					fontColor.switchToColor();
				myEntries[but]->render(0);

				buttonLength  = aveCharWidth * (float)myEntries[but]->getLabel().length();
				buttonLength += MENU_DISTANCE;
				buttonLength /= 2.0f;

				glTranslatef(buttonLength, 0.0f, 0.0f);
				if(but % 2)
					glTranslatef(0.0f, MENU_DISTANCE, 0.0f);
				else
					glTranslatef(0.0f, -MENU_DISTANCE, 0.0f);
			}
			glPopMatrix();
		}

		// player list
		{
			glPushMatrix();
			glLoadIdentity();
			glTranslatef(MENU_COORD_X, MENU_COORD_Y - (2.0f * MENU_DISTANCE), MENU_COORD_Z);
			unsigned int offset = (unsigned int)myEntries.size();
			for(unsigned int i = 0; i < playerList->size(); i++) {
				if(i == 8)
					glTranslatef(-MENU_COORD_X, 0.0f, 0.0f);	// Start a second column, so move right
				std::string buffer = (*playerList)[i]->getName();
				if((i + offset) == (unsigned int)activeIndex) {
					activeColor.switchToColor();
					if(!isClientLobby())
						buffer.append("  <- *kick*");
				}
				else
					fontColor.switchToColor();

				menuEntry current(buffer, i, NULL);
				unsigned int idx = i;
				if(idx >= 8)
					idx -= 8;
				current.render(idx);

				if((*playerList)[i]->getReadyFlag())
					fontColor.switchToColor();
				else
					activeColor.switchToColor();
				glPushMatrix();
				glTranslatef(-0.1f, -(idx * MENU_DISTANCE) + (MENU_DISTANCE / 3.0f), 0.0f);
				glutSolidSphere(0.05, (int)GLOBAL_LEVEL_OF_DETAIL, (int)GLOBAL_LEVEL_OF_DETAIL);
				glPopMatrix();
			}
			glPopMatrix();
		}

		// Render Mini-Avatar
		{
			float rotMat[16];
			glGetFloatv(GL_MODELVIEW_MATRIX, rotMat);
			glPushMatrix();
			if(isEnTex) glEnable(GL_TEXTURE_2D);
			glLoadIdentity();
			glTranslatef(-(2*MENU_DISTANCE), MENU_COORD_Y - (2 * MENU_DISTANCE), MENU_COORD_Z - MENU_DISTANCE);
			for(unsigned int i = 0; i < playerList->size(); i++) {
				if(i == 8)
					glTranslatef(-MENU_COORD_X, 0.0f, 0.0f);
				unsigned int idx = i;
				if(idx >= 8)
					idx -= 8;
				glPushMatrix();
				glTranslatef(0.0f, -(idx * MENU_DISTANCE), 0.0f);
				//glTranslatef(0.0f, (MENU_DISTANCE / 3.0f), 0.0f);
				glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
				glMultMatrixf(rotMat);
				glScalef(0.08f, 0.08f, 0.08f);
				Vector3D pos = (*playerList)[i]->getPosition() * -1.0f;
				glTranslatef(pos.x, pos.y, pos.z);
					// *Player*::render(), damit keine Namen etc. angezeigt werden
				(*playerList)[i]->Player::render();
				glPopMatrix();
			}
			glDisable(GL_TEXTURE_2D);
			glPopMatrix();
		}

		// if server: display IP-addresses
		if(!isClient) {
			fontColor.switchToColor();
			{
				std::string ipTitle = "lokale IP-Adresse";
				if(ipAddressList.size() > 1)
					ipTitle.append("n");
				ipTitle.append(":");
				glPushMatrix();
				glLoadIdentity();
				glTranslatef(MENU_COORD_X, MENU_COORD_Y - MENU_DISTANCE, MENU_COORD_Z);
				glScalef(0.0005f, 0.0005f, 0.0005f);
				for(unsigned int c = 0; c <= ipTitle.length(); c++)
					glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)ipTitle[c]);
				glPopMatrix();
			}
			for(unsigned int i = 0; i < ipAddressList.size(); i++) {
				unsigned int len = (unsigned int)ipAddressList[i].length();
				glPushMatrix();
				glLoadIdentity();
				glTranslatef(MENU_COORD_X + (i*0.5f) + 0.7f, MENU_COORD_Y - MENU_DISTANCE, MENU_COORD_Z);
				glScalef(0.0005f, 0.0005f, 0.0005f);
				for(unsigned int c = 0; c <= len; c++)
					glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)ipAddressList[i][c]);
				glPopMatrix();
			}
		}

		// if available, display the message
		if(!msgText.empty()) {
			titleColor.switchToColor();
			glPushMatrix();
			glLoadIdentity();
			glTranslatef(MENU_COORD_X, MENU_COORD_Y - (12.3f * MENU_DISTANCE), MENU_COORD_Z);
			glScalef(0.0005f, 0.0005f, 0.0005f);
			for(unsigned int c = 0; c <= msgText.length(); c++)
				glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)msgText[c]);
			glPopMatrix();
		}

		if(isEnTex) glEnable(GL_TEXTURE_2D);
		glDisable(GL_NORMALIZE);
	}
}

void lobbyMenu::dispatchCallback(unsigned int index) const {
	if(index >= myEntries.size() + playerList->size())
		return;
	if(index < myEntries.size())	// Usual Entry
		menu::dispatchCallback(index);
	else if(myCallbackFunc) {
		(*myCallbackFunc)(const_cast<lobbyMenu*>(this), index);
	}
}

//void lobbyMenu::setReadyFlags(int idx, bool isReady) {
//	if(isReady)
//		playerReadyFlags |= (1<<idx);
//	else
//		playerReadyFlags &= ~(1<<idx);
//}