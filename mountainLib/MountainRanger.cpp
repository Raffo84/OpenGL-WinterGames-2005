#include "MountainRanger.h"

MountainRanger::MountainRanger(void)
{
#ifdef _DEBUG
	std::cout << "\tMountainRanger:\n";
#endif
	srand((unsigned int)time(NULL));
	//if(timeBeginPeriod(TIMER_RESOLUTION) == TIMERR_NOCANDO) {
	//	std::cerr << "FEHLER: Kann den Timer nicht auf >";
	//	std::cerr << TIMER_RESOLUTION << "< einstellen!\n";
	//}

	std::cout << "\t\tlade Spieler...\n";
	playerList.push_back(new HumanPlayer());
	playerList[0]->setName("Player");
	playerList[0]->setLOD(GLOBAL_LEVEL_OF_DETAIL);
	setCamera(CAM_HEAD);
	IPAddress = "127.0.0.1";
	hsFreestyle.value	= 0.0;
	hsFreestyle.name	= std::string("Niemand");
	hsDownhill.value	= 180.0;
	hsDownhill.name		= std::string("Niemand");
	hsSlalom.value		= 180.0;
	hsSlalom.name		= std::string("Niemand");
	changeResolution(RESOLUTION_LOW_WIDTH, RESOLUTION_LOW_HEIGHT);
	loadConfig();
}

MountainRanger::~MountainRanger(void)
{
	for(unsigned int i = 0; i < playerList.size(); i++) {
		delete playerList[i];
	}
	for(unsigned int i = 0; i < gates.size(); i++) {
		delete gates[i];
	}
	delete horn;
	delete pEmitter;
	delete height;
	delete speed;
	delete wind;
	delete movement;
	delete rotation;

	//timeEndPeriod(TIMER_RESOLUTION);
}

void MountainRanger::start() {
	quadRenderNumber = new unsigned int;
	*quadRenderNumber = 0;
	quadRenderList = new unsigned int[6400];
	packetProcessor = new PlayerPacketProcessor();
#ifdef _DEBUG
	std::cout << "\t\tlade Matterhorn...\n";
#else
	std::cout << "\t\tlade Umgebung...\n";
#endif
	horn = new Matterhorn(quadRenderList, quadRenderNumber);
	obstacleSeed = (unsigned int)time(NULL);

#ifdef _DEBUG
	std::cout << "\t\tlade Partikel Emitter...\n";
#else
	std::cout << "\t\tlade Wettersystem...\n";
#endif
#ifdef _DEBUG
	height = new Vector3D(0.0f, 20.0f, 0.0f);
	speed = new Vector3D(0.0f, -0.7f, 0.0f);
	wind = new Vector3D(0.0f, 0.0f, 0.0f);
	float* density = new float;
	*density = 0.1f;	
#else
	height = new Vector3D(0.0f, 75.0f, 0.0f);
	speed = new Vector3D(0.0f, -0.7f, 0.0f);
	wind = new Vector3D(0.0f, 0.0f, 0.0f);
	float* density = new float;
	*density = 0.1f;
#endif
	pEmitter = new ParticleEmitter(density, height, speed, wind, movement, horn, quadRenderList, quadRenderNumber);

	for(unsigned int i = 0; i < (unsigned int)playerList.size(); i++)
		playerList[i]->myChar->flush();

	lastUpdate = clock();
	timeDiff = 0.0f;
	lastNetworkUpdateSpeedAngle		= 0.0f;
	lastNetworkUpdateHeading		= 0.0f;
	lastNetworkUpdatePosition		= 0.0f;
	lastNetworkUpdateStylePoints	= 0.0f;
	lastNetworkPing					= 0.0f;
	gameStatusFlags = GAME_STATUS_NONE;
	currentGateIndicatorDL = 0;
	keyboardTarget = NULL;
	configWasChanged = false;
	getScreenMetrics(&currentResWidth, &currentResHeight);

	camPlayerNum	= 0;
	lastPlayerViewChange = 0;
	movement		= new Vector3D(0.0f, 0.0f, 0.0f);
	rotation		= new Vector3D(0.0f, 0.0f, 0.0f);
	targetRotation	= Vector3D(0.0f, 0.0f, 0.0f);
	camDisplacement	= Vector3D(0.0f, 0.0f, 0.0f);
	menuCam			= Vector3D(0.0f,-30.0f, 140.0f);
	menuRot			= Vector3D(0.0f, 0.0f, 0.0f);
	HUDcol			= Color(HUD_COLOR_R, HUD_COLOR_G, HUD_COLOR_B);

	buildMainMenu();
	activateMainMenu();
}

void MountainRanger::keyboardHandler(int key, int modi, bool isSpecial) {
	bool menuIsActive = gameStatusFlags & GAME_STATUS_MENU;
	clock_t now = clock();
	timeDiff = (float)(now - lastUpdate) / (float)CLK_TCK;

	if(isSpecial) {
		switch(key) {
		case GLUT_KEY_UP:
			if(menuIsActive) {
				if(keyboardTarget)
					mainMenu.fire();
				mainMenu.changeSelection(-1);
			}
			else {
				if(playerList[0]->getSpeed() < 1.4f && !playerList[0]->isStopped()) {
					// Wir sind fair: unter 5km/h darf man sich "anschubsen"
					accPlayer(1.4f);	// 1.4 m/s == 5.04 km/h
				}
				else if(!playerList[0]->isStopped()){
					// um bewusst abzuspringen
					playerList[0]->jump();
				}
			}
			break;
		case GLUT_KEY_DOWN:
			if(menuIsActive) {
				if(keyboardTarget)
					mainMenu.fire();
				mainMenu.changeSelection(1);
			}
			else if(!playerList[0]->isFlying() && !playerList[0]->isStopped()){
				// bremsen
				accPlayer(-timeDiff * BRAKE_RATE);
			}
			break;
		case GLUT_KEY_LEFT:
			if(!menuIsActive)
				turnPlayer(timeDiff * ROTATION_RATE);
			else
				mainMenu.changeSelection(-1);
			break;
		case GLUT_KEY_RIGHT:
			if(!menuIsActive)
				turnPlayer(-timeDiff * ROTATION_RATE);
			else
				mainMenu.changeSelection(1);
			break;
		case GLUT_KEY_F1:	// switch to close 3rd-person-view ("Eye Cam")
			if(!menuIsActive)
				setCamera(CAM_EYE);
			break;
		case GLUT_KEY_F2:	// switch to medium 3rd-person-view ("Head Cam")
			if(!menuIsActive)
				setCamera(CAM_HEAD);
			break;
		case GLUT_KEY_F3:	// switch to far 3rd-person-view ("Heli Cam")
			if(!menuIsActive)
				setCamera(CAM_HELI);
			break;
		case GLUT_KEY_F4:
			if(modi & GLUT_ACTIVE_ALT)		// Alt+F4, close app, wouldn't work else
				quitGame(0);
			break;
		default:
#ifdef _DEBUG
			std::cout << "unknown special key: " << key << " (\"" << (char)key << "\")\n";
#endif
			break;
		}
	}
	else {
		if(keyboardTarget) {
			switch(key) {
			case 27:	// ESC
				// aktuellen Wert auf vorherigen zurücksetzen
				keyboardInput.clear();
				keyboardInput = keyboardTargetOldValue;
				{
					std::string buffer = keyboardTargetDefault;
					buffer.append(keyboardInput);
					buffer.append(1, MENU_CURSOR_CHAR);
					keyboardTarget->setLabel(buffer);
				}
				break;
			case 13:	// RETURN
				// Nichts muss gemacht werden, wird vom MenuHandler gemacht
				mainMenu.fire();
				break;
			case 8:		// BACKSPACE
				// letztes Zeichen löschen
				if(keyboardInput.length() > 0) {
					keyboardInput.erase(keyboardInput.length() - 1, 1);
					{
						std::string buffer = keyboardTargetDefault;
						buffer.append(keyboardInput);
						buffer.append(1, MENU_CURSOR_CHAR);
						keyboardTarget->setLabel(buffer);
					}
				}
				break;
			default:
				// Buchstaben an keyboardInput senden
				keyboardInput.append(1, (char)key);
				{
					std::string buffer = keyboardTargetDefault;
					buffer.append(keyboardInput);
					buffer.append(1, MENU_CURSOR_CHAR);
					keyboardTarget->setLabel(buffer);
				}
				break;
			}
		}
		else {
			switch(key) {
			case 27:	// ESC
				if(!menuIsActive)
					activateMainMenu();
				else {
					clearPlayerList();
					if(gameStatusFlags & GAME_STATUS_LAN_RUNNING) {
						gameStatusFlags = GAME_STATUS_NONE;
						activateMainMenu();
					}
					else {
						menu* par = mainMenu.getCurrentMenu()->getParent();
						if(par != NULL) {
							mainMenu.getCurrentMenu()->hide();
							par->show();
						}
						gameStatusFlags  = GAME_STATUS_NONE;
						gameStatusFlags |= GAME_STATUS_MENU;
						setNightFlag();
						//updateLigthing();
					}
				}
				break;
			case 13:	// RETURN
				if(menuIsActive)
					mainMenu.fire();
				else if(gameStatusFlags & GAME_STATUS_FINISHED) {
					if(runClient()) {
						playerList[0]->setReadyFlag(true);
						sendInfoReadyFlag(0);
					}
					else if(runServer()) {
						if(networkAllPlayersReady()) {
							activateMainMenu();
							// send to clients
							ProtLobby *protLobby = new ProtLobby();
							protLobby->giveInformation(0);
							packetProcessor->sendPacket(MAX_PLAYERS, protLobby);
						}
					}
					else
						activateMainMenu();
				}
				break;
			case 22:	// CTRL + 'V'
				if(!gameIsRunning() && (modi & GLUT_ACTIVE_CTRL)) {
					std::stringstream buffer;
					buffer << "V1." << CURRENT_VERSION;
					mainMenu.getCurrentMenu()->displayMessage(buffer.str());
				}
				break;
			case '+':
				if(gameIsRunning() && ((float)(clock() - lastPlayerViewChange) / CLK_TCK) > 0.1f) {
					camPlayerNum++;
					if(camPlayerNum >= (int)playerList.size())
						camPlayerNum = 0;
					//float newAngle  = playerList[camPlayerNum]->getHeading().getAngle(Vector3D(-1.0f, 0.0f, 0.0f));
					//float helpAngle = playerList[camPlayerNum]->getHeading().getAngle(Vector3D(0.0f, 0.0f, -1.0f));
					//if(helpAngle > 90.0f)
					//	newAngle *= -1.0f;
					////delete rotation;		// BUGGY!!!
					////*rotation = new Vector3D(0.0f, newAngle - 90.0f, 0.0f);
					////targetRotation.clear();
					lastPlayerViewChange = clock();

					_ASSERT(camPlayerNum >= 0);
					_ASSERT(camPlayerNum < (int)playerList.size());
				}
				break;
			case '-':
				if(gameIsRunning() && ((float)(clock() - lastPlayerViewChange) / CLK_TCK) > 0.1f) {
					camPlayerNum--;
					if(camPlayerNum < 0) {
						camPlayerNum = (unsigned int)playerList.size();
						camPlayerNum--;
					}
					lastPlayerViewChange = clock();

					_ASSERT(camPlayerNum >= 0);
					_ASSERT(camPlayerNum < (int)playerList.size());
				}
				break;
			case '*':
				if(gameIsRunning())
					camPlayerNum = 0;
				break;
#ifdef _DEBUG
			case 'r':
				{
					std::clog << "\n *** FULL PLAYER REPORT ***\n";
					unsigned int numBots = 0, numLAN = 0;
					for(unsigned int i = 0; i < playerList.size(); i++) {
						playerList[i]->fullReport();
						if(playerList[i]->getPlayerType() == PLAYER_TYPE_AI)
							numBots++;
						else if(playerList[i]->getPlayerType() == PLAYER_TYPE_LAN)
							numLAN++;
					}
					std::clog << "Total Number of Players:\t" << (unsigned int)playerList.size() << '\n';
					std::clog << "\tdavon KI-Spieler:\t" << numBots << '\n';
					std::clog << "\tdavon LAN-Spieler:\t" << numLAN << '\n';
					std::clog << " *** END ***\n\n";
				}
				break;
#endif
			default:
#ifdef _DEBUG
				std::cout << "unknown key: " << key << " (\"" << (char)key << "\")\n";
#endif
				break;
			}
		}
	}
}

void MountainRanger::update()
{
	//DWORD now = timeGetTime();
	//while (now == lastUpdate) {
	//	now = timeGetTime();
	//	std::cerr << "\n>>> ACHTUNG: kuenstliche Verzoegerung! <<<\n\n";
	//}
	//timeDiff = (float)(now - lastUpdate) / 1000.0f;
	clock_t now = clock();
	while (now == lastUpdate)
		now = clock();
	timeDiff = (float)(now - lastUpdate) / (float)CLK_TCK;
	lastUpdate = now;

	{
		float deg = timeDiff * CAM_MAX_ROTATION_RATE;
		Vector3D camRotDiff = targetRotation;
		// X
		if(camRotDiff.x > deg)
			camRotDiff.x = deg;
		else if(camRotDiff.x < -deg)
			camRotDiff.x = -deg;
		// Y
		if(camRotDiff.y > deg)
			camRotDiff.y = deg;
		else if(camRotDiff.y < -deg)
			camRotDiff.y = -deg;
		// Z
		if(camRotDiff.z > deg)
			camRotDiff.z = deg;
		else if(camRotDiff.z < -deg)
			camRotDiff.z = -deg;

		*rotation = *rotation + camRotDiff;
		targetRotation = targetRotation - camRotDiff;
	}

	glPushMatrix();
	//if(!(gameStatusFlags & GAME_STATUS_MENU)) {
	if(gameIsRunning()) {
		if(gameStatusFlags & GAME_STATUS_COUNTDOWN) {
			countdown -= timeDiff;
			if(countdown <= 0) {
				countdown = 0.0f;
				gameStatusFlags &= ~GAME_STATUS_COUNTDOWN;
				for(unsigned int i = 0; i < playerList.size(); i++) {
					startTime[i] = clock();
					finalTime[i] = 0;
					//style[i] = 0;
					playerList[i]->restart();
					accPlayer(1.4f, i);	// ~ 5km/h
				}
			}
		}

		updatePlayers();
				
		Vector3D playerPos = playerList[camPlayerNum]->getPosition();
		Vector3D playerAir = playerList[camPlayerNum]->getAirHeading();
		//Vector3D playerHeading = playerList[0]->getHeading();

		glTranslatef(camDisplacement.x, camDisplacement.y, camDisplacement.z);
		glTranslatef(-camPos.x, -camPos.y, -camPos.z);
		glTranslatef(movement->x, movement->y, movement->z);

		Vector3D virtualCam;
		{
			virtualCam = playerPos;
			Matrix3D rotMat;
			rotMat.createRotationMatrix(0.0f, -rotation->y, 0.0f);
			Vector3D rotatedPos = rotMat * camPos;	//(camPos - camDisplacement);
			virtualCam += rotatedPos;
			float virtualCamHeight = horn->getHeightOverGround(virtualCam);
			if(virtualCamHeight < CAM_MIN_HEIGHT) {
				virtualCam.y += CAM_MIN_HEIGHT;
				virtualCam.y -= virtualCamHeight;
			}
			virtualCam -= playerPos;
			virtualCam.normalize();
			virtualCam *= -camPos.length();
		}
		float camAngle = virtualCam.getAngle(Vector3D(virtualCam.x, 0.0f, virtualCam.z));
		if(camAngle < 40.0f && !(gameStatusFlags & GAME_STATUS_FINISHED))
			camAngle = 40.0f;
		glRotatef(camAngle, 1.0f, 0.0f, 0.0f);

		//glGetFloatv(GL_MODELVIEW_MATRIX, modl);
		//std::clog << "3: ";
		//std::clog << modl[12] << " -- " << modl[13] << " -- ";
		//std::clog << modl[14] << " -- " << modl[15] << std::endl;

		glRotatef(rotation->x, 1.0f, 0.0f, 0.0f);
		glRotatef(rotation->y, 0.0f, 1.0f, 0.0f);
		glRotatef(rotation->z, 0.0f, 0.0f, 1.0f);
		////Obsolete through virtualCam rotation
		//float incl = 0.0f;
		//if(!(gameStatusFlags & GAME_STATUS_FINISHED)) {
		//	incl = playerAir.getAngle(Vector3D(playerAir.x, 0.0f, playerAir.z));
		//	if(playerAir.y > 0)
		//		incl *= -1.0f;
		//	glRotatef(incl, 1.0f, 0.0f, 0.0f);
		//}
		glTranslatef(-playerPos.x, -playerPos.y, -playerPos.z);

		horn->renderWithQuads(playerList[camPlayerNum]->getPosition());
		pEmitter->update(timeDiff);
		for(unsigned int i = 0; i < playerList.size(); i++)
			playerList[i]->render();
		for(unsigned int i = 0; i < obstacles.size(); i++)
			obstacles[i]->render();

		// Spieltyprelevante Objekte anzeigen
		if(gameStatusFlags & GAME_STATUS_TYPE_SLALOM) {
			for (unsigned int i = 0; i < gates.size(); i++) {
				gates[i]->render();
			}
			if(!(gameStatusFlags & GAME_STATUS_COUNTDOWN))
				gates[currentGate]->update(timeDiff);
			checkGates();

			// Richtungsanzeiger zeichnen
			if(!(gameStatusFlags & GAME_STATUS_FINISHED)) {
				Vector3D dir	= gates[currentGate]->getPosition() - playerPos;
				Vector3D axis	= Vector3D(dir, Vector3D(0.0f, 0.0f, -1.0f));
				float angle		= dir.getAngle(Vector3D(0.0f, 0.0f, -1.0f));
				dir.normalize();
				glPushMatrix();
				//glLoadIdentity();
				//glTranslatef(-camPos.x, -camPos.y, -camPos.z);
				//glTranslatef(0.0f, AVATAR_SIZE + 0.8f, 0.0f);
				glTranslatef(playerPos.x, playerPos.y, playerPos.z);
				glTranslatef(0.0f, AVATAR_SIZE + 0.8f, 0.0f);
				glRotatef(angle, axis.x, axis.y, axis.z);
				if(glIsList(currentGateIndicatorDL + 1))
					glCallList(currentGateIndicatorDL + 1);
				glPopMatrix();
			}
		}

		// Am Ende des Spiels
		if(gameStatusFlags & GAME_STATUS_FINISHED) {
			rotateAll(0.0f, MENU_ROTATION_RATE * timeDiff, 0.0f);
		}
	}
	else {
		int lobbyFlags = gameStatusFlags & GAME_STATUS_TYPE_ALL;
		lobbyFlags |= (gameStatusFlags & GAME_STATUS_NIGHT);
		lobbyFlags |= (gameStatusFlags & GAME_STATUS_DAY);
		lanLobby->setGameType(lobbyFlags);

		rotateAll(0.0f, MENU_ROTATION_RATE * timeDiff, 0.0f);
		// Hintergrund zeichnen
		glPushMatrix();
		glTranslatef(-menuCam.x, -menuCam.y, -menuCam.z);
		glTranslatef(movement->x, movement->y, movement->z);
		glRotatef(menuRot.x, 1.0f, 0.0f, 0.0f);
		glRotatef(menuRot.y, 0.0f, 1.0f, 0.0f);
		glRotatef(menuRot.z, 0.0f, 0.0f, 1.0f);
		horn->render();
		glPopMatrix();

		// Menü anzeigen
		glRotatef(menuRot.y, 0.0f, 1.0f, 0.0f);
		mainMenu.render();

		if(gameStatusFlags & GAME_STATUS_MENU_PLAYER) {
			Vector3D pPos, pRot, pAir;
			glPushMatrix();
			glLoadIdentity();
			glTranslatef(1.5f, -1.0f, -6.0f);
			glRotatef(10.0f, 1.0f, 0.0f, 0.0f);
			glRotatef(menuRot.x, 1.0f, 0.0f, 0.0f);
			glRotatef(menuRot.y, 0.0f, 1.0f, 0.0f);
			glRotatef(menuRot.z, 0.0f, 0.0f, 1.0f);

			// invert current player position
			pPos = playerList[camPlayerNum]->getPosition();
			pRot = playerList[camPlayerNum]->getRotation();
			pAir = playerList[camPlayerNum]->getAirHeading();

			float incl = pAir.getAngle(Vector3D(pAir.x, 0.0f, pAir.z));
			if(pAir.y > 0)
				incl *= -1.0f;
			glRotatef(-incl, 0.0f, 0.0f, 1.0f);
			glRotatef(-pRot.x, 1.0f, 0.0f, 0.0f);
			glRotatef(-pRot.y, 0.0f, 1.0f, 0.0f);
			glRotatef(-pRot.z, 0.0f, 0.0f, 1.0f);
			glTranslatef(-pPos.x, -pPos.y, -pPos.z);

			playerList[camPlayerNum]->render();
			glPopMatrix();
		}
	}
	glPopMatrix();

	// updating network
	if (gameStatusFlags & GAME_STATUS_LANGAME) {
        sendSelfAndAI(timeDiff);
	}
	bool server = false;
	if (gameStatusFlags & GAME_STATUS_SERVER) server = true;
    packetProcessor->update(timeDiff, server);

	renderHUD();
}

void MountainRanger::moveAll(float x, float y, float z)
{
	movement->x += x;
	movement->y += y;
	movement->z += z;
}

void MountainRanger::rotateAll(float x, float y, float z) {
	if(gameStatusFlags & GAME_STATUS_MENU) {
		menuRot.x += x;
		menuRot.y += y;
		menuRot.z += z;

		if(menuRot.x > 180.0f)
			menuRot.x -= 360.0f;
		if(menuRot.y > 180.0f)
			menuRot.y -= 360.0f;
		if(menuRot.z > 180.0f)
			menuRot.z -= 360.0f;
	}
	else {
		targetRotation.x += x;
		targetRotation.y += y;
		targetRotation.z += z;

		if(targetRotation.x > 180.0f)
			targetRotation.x -= 360.0f;
		if(targetRotation.y > 180.0f)
			targetRotation.y -= 360.0f;
		if(targetRotation.z > 180.0f)
			targetRotation.z -= 360.0f;

		if(targetRotation.x < -180.0f)
			targetRotation.x += 360.0f;
		if(targetRotation.y < -180.0f)
			targetRotation.y += 360.0f;
		if(targetRotation.z < -180.0f)
			targetRotation.z += 360.0f;
	}
}


void MountainRanger::turnPlayer(float angle, unsigned int index) {
	if(index < playerList.size() && !(gameStatusFlags & GAME_STATUS_FINISHED)) {
		playerList[index]->turnPlayer(angle);

		if((gameStatusFlags & GAME_STATUS_TYPE_FREE) && playerList[index]->getPlayerType() != PLAYER_TYPE_LAN)
			style[index]->incAngle(angle);

		// local player, adjust camera
		if(index == 0) // && camPlayerNum == 0)
			rotateAll(0.0f, -angle, 0.0f);
	}
}

void MountainRanger::updatePlayers() {
	bool stoppedRightNow = false;
	bool playerWasFlying = playerList[camPlayerNum]->isFlying();
	unsigned int size = (unsigned int)playerList.size();
	unsigned int obsSize = (unsigned int)obstacles.size();
	for(unsigned int i = 0; i < size; i++) {
		playerList[i]->setCamRot(*rotation);
		if(!playerList[i]->isStopped()) {
			Vector3D normal = horn->getNormal(playerList[i]->getPosition());
			playerList[i]->update(timeDiff, normal);
		}
		float height = horn->getHeightOverGround(playerList[i]->getPosition());
		playerList[i]->fall(timeDiff, height);
		// Check for Collisions
		if (!runClient()) {
            for(unsigned int j = (i+1); j < size; j++) {
				if(*playerList[i] && playerList[j]) {
					// singleplayer or server
					playerList[i]->crashAgainst(playerList[j]->getPosition());
					playerList[j]->crashAgainst(playerList[i]->getPosition());
					if (gameStatusFlags & GAME_STATUS_SERVER) {
						// server
						ProtCrash *protCrash = new ProtCrash();
						protCrash->giveInformation(i, j, ACTOR_TYPE_PERSON);
						packetProcessor->sendPacket(MAX_PLAYERS, protCrash);
					}
				}
			}

			for(unsigned int o = 0; o < obsSize; o++) {
				if(*playerList[i] && obstacles[o]) {
					playerList[i]->crashAgainst(obstacles[o]->getPosition());
					if (gameStatusFlags & GAME_STATUS_SERVER) {
							// server
							ProtCrash *protCrash = new ProtCrash();
							protCrash->giveInformation(i, o, ACTOR_TYPE_OBSTACLE);
							packetProcessor->sendPacket(MAX_PLAYERS, protCrash);
					}
				}
			}
		}

		// check if jump bonus is set
		bool jumpBonus = playerList[i]->isNewBonus();
		if (jumpBonus && (gameStatusFlags & GAME_STATUS_LANGAME)) {
			if (!(playerList[i]->getPlayerType() == PLAYER_TYPE_LAN)) {
				// sending all players the new "lastFallingRate"
				ProtLastFallSpeed *protLastFallSpeed = new ProtLastFallSpeed();
				protLastFallSpeed->giveInformation(packetProcessor->serverPlayerNum(i), playerList[i]->getCurrentJumpBonus() * MATTERHORN_MAX_JUMP_BONUS);
				packetProcessor->sendPacket(MAX_PLAYERS, protLastFallSpeed);
			}
		}

		// update Style-Points
		if((gameStatusFlags & GAME_STATUS_TYPE_FREE) && playerList[i]->getPlayerType() != PLAYER_TYPE_LAN) {	// GAME_STATUS_TYPE_SLALOM) && !(gameStatusFlags & GAME_STATUS_TYPE_DOWN)) {
			if(playerList[i]->isFlying())
				style[i]->incPoints(timeDiff);
			else if(style[i]->isActive(timeDiff) == stylePoints::ACTIVE_COUNTING) {
				// check if player just crashed
				bool crashed = false;
				if(playerList[i]->getSpeed() < 0.1f)
					crashed = true;
				style[i]->updatePoints(crashed);
			}
			style[i]->incHeight(horn->getHeightOverGround(playerList[i]->getPosition()));
			if(jumpBonus) {
				if(playerList[i]->getCurrentJumpBonus() >= MONSTER_JUMP_PERCENT)
					style[i]->incExtraPoints(MONSTER_JUMP_POINTS);
			}
		}
		if(!playerList[i]->isStopped()) {
			if(gameStatusFlags & GAME_STATUS_TYPE_FREE) {
				float runTime = (float)(clock() - startTime[i]);
				runTime /= CLK_TCK;
				if(runTime > FREESTYLE_DURATION) {
					playerList[i]->stop();
					stoppedRightNow = true;
				}
			}
			//if((abs(playerList[i]->getPosition().x) > 940 || abs(playerList[i]->getPosition().z) > 970)) {
			if(playerList[i]->getPosition().y < TRACK_BORDER) {
				playerList[i]->stop();
				stoppedRightNow = true;
			}
		}
		if(stoppedRightNow) {
			if(i == 0)
				gameStatusFlags |= GAME_STATUS_FINISHED;
			if(runServer()) {
				playerList[0]->setReadyFlag(true);
				sendInfoReadyFlag(0);
			}
			if(gameStatusFlags & GAME_STATUS_TYPE_DOWN) {
				finalTime[i] = (double)(clock() - startTime[i]) / CLK_TCK;
				if(finalTime[i] <= hsDownhill.value) {
					hsDownhill.value = finalTime[i];
					hsDownhill.name  = playerList[i]->getName();
					configWasChanged = true;
					saveConfig();
				}
			}
			else if(gameStatusFlags & GAME_STATUS_TYPE_SLALOM) {
				finalTime[i] = (double)(clock() - startTime[i]) / CLK_TCK;
				for(unsigned int j = currentGate; j < gates.size(); j++) {
					// sum all remaining time-outs
					// and mark all remaining gates as missed
					finalTime[i] += gates[j]->getTimeOut();
					gates[j]->activate(false);
					gates[j]->setTimeOut(0.0f);
				}
				int missedGates = 0;
				for(unsigned int j = 0; j < gates.size(); j++) {
					if(gates[j]->isActive() & SLALOM_GATE_TIMED_OUT)
						missedGates++;
				}
				double timePenalty = SLALOM_GATE_MISS_PENALTY * missedGates;
				finalTime[i] += timePenalty;
				if(finalTime[i] <= hsSlalom.value) {
					hsSlalom.value = finalTime[i];
					hsSlalom.name = playerList[i]->getName();
					configWasChanged = true;
					saveConfig();
				}
			}
			else if(gameStatusFlags & GAME_STATUS_TYPE_FREE) {
				if(playerList[i]->getPlayerType() != PLAYER_TYPE_LAN)
					style[i]->updatePoints();
				if(style[i]->getTotalPoints() >= hsFreestyle.value) {
					hsFreestyle.value = (double)style[i]->getTotalPoints();
					hsFreestyle.name  = playerList[i]->getName();
					configWasChanged = true;
					saveConfig();
				}
			}
		}
		stoppedRightNow = false;
	}
	// Check new speed and adjust camDisplacement
	if(playerList[camPlayerNum]->getSpeed() < CAM_DISPLACEMENT_MIN_SPEED
		|| playerList[camPlayerNum]->isFlying()) {
		//camDisplacement = Vector3D(0.0f, 0.0f, 0.0f);
		if(!camDisplacement.isNull()) {
			Vector3D diff = camDisplacement;
			diff.normalize();
			diff *= timeDiff;
			if(abs(diff.x) > abs(camDisplacement.x))
				diff.x = camDisplacement.x;
			if(abs(diff.y) > abs(camDisplacement.y))
				diff.y = camDisplacement.y;
			diff *= -1.0f;
			camDisplacement += diff;
		}
	}
	else {
		Vector3D disp = Vector3D((float)(rand() - (RAND_MAX / 2)) / 2.0f, (float)(rand() - (RAND_MAX/2)), 0.0f);
		disp.normalize();
		if(playerWasFlying) {
			// We have just landed, so bob down the camera
			disp = Vector3D(0.0f, 1.0f, 0.0f);
			disp *= (1.0f / timeDiff);	// Get to the lowest possible Point immediately
		}
		float speedFactor = playerList[camPlayerNum]->getSpeed() - CAM_DISPLACEMENT_MIN_SPEED;
		speedFactor /= CAM_DISPLACEMENT_STEP;
		//speedFactor *= timeDiff;
		disp *= speedFactor;
		disp *= timeDiff;
		camDisplacement += disp;
		if(camDisplacement.length() > speedFactor) {
			float newLen = (2*speedFactor) - camDisplacement.length();
			camDisplacement.normalize();
			camDisplacement *= newLen;
		}
	}
}

void MountainRanger::accPlayer(float factor, unsigned int index) {
	if(index < playerList.size())
		playerList[index]->accPlayer(factor);
}

void MountainRanger::setCamera(int index) {
	Vector3D newCameraAngle;
	switch(index) {
		case CAM_EYE:
			newCameraAngle = VIEW_EYE_CAM;
			break;
		case CAM_HELI:
			newCameraAngle = VIEW_HELI_CAM;
			break;
		default:
			std::cerr << "Falscher Kamera-Winkel! Verwende Head-Cam!\n";
		case CAM_HEAD:
			newCameraAngle = VIEW_HEAD_CAM;
			break;
	}
	if(camPos != newCameraAngle) {
		camPos = newCameraAngle;
		configWasChanged = true;
	}
}

void MountainRanger::updateLigthing() {
	if(gameStatusFlags & GAME_STATUS_NIGHT) {
		glDisable(GL_LIGHT1);
		glEnable(GL_LIGHT2);
		glFogfv(GL_FOG_COLOR, colFogNight);
		glClearColor(colBackgroundNight.r, colBackgroundNight.g, colBackgroundNight.b, 0.0f);

		HUDcol.r = HUD_COLOR_R_N;
		HUDcol.g = HUD_COLOR_G_N;
		HUDcol.b = HUD_COLOR_B_N;
		mainMenu.setColors(Color(0.7f, 1.0f, 0.4f), Color(1.0f, 0.5f, 0.5f), Color(0.0f, 1.0f, 0.8f));
	}
	else {
		glEnable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
		glFogfv(GL_FOG_COLOR, colFogDay);
		glClearColor(colBackgroundDay.r, colBackgroundDay.g, colBackgroundDay.b, 0.0f);

		HUDcol.r = HUD_COLOR_R;
		HUDcol.g = HUD_COLOR_G;
		HUDcol.b = HUD_COLOR_B;
		mainMenu.setColors(Color(0.0f, 0.7f, 0.2f), Color(1.0f, 0.0f, 0.0f), Color(0.0f, 0.0f, 1.0f));
	}
}

void MountainRanger::setNightFlag() {
	SYSTEMTIME zeit;
	GetLocalTime(&zeit);
	if(zeit.wHour >= 20 || zeit.wHour <= 5)	// TODO: An Jahreszeiten anpassen
		gameStatusFlags |=  GAME_STATUS_NIGHT;
	else
		gameStatusFlags &= ~GAME_STATUS_NIGHT;
	updateLigthing();
}

void MountainRanger::activateMainMenu() {
	if(!(gameStatusFlags & GAME_STATUS_MENU)) {
		saveConfig();

		// Gates löschen
		for(unsigned int i = 0; i < gates.size(); i++)
			delete gates[i];
		gates.clear();
		glDeleteLists(currentGateIndicatorDL, 1);
		// Hindernisse löschen
		for(unsigned int i = 0; i < obstacles.size(); i++)
			delete obstacles[i];
		obstacles.clear();

		if(!(gameStatusFlags & GAME_STATUS_LANGAME)) {
			clearPlayerList();
			setNightFlag();
			//updateLigthing();
			mainMenu.reset();
		}
		else {
			// reset values
			for(unsigned int i = 0; i < startTime.size(); i++)
				startTime[i] = 0;
			for(unsigned int i = 0; i < finalTime.size(); i++)
				finalTime[i] = 0.0;
			for(unsigned int i = 0; i < style.size(); i++)
				style[i]->reset();
		}

		for(unsigned int i = 0; i < (unsigned int)playerList.size(); i++) {
			// Spieler zurücksetzen - hier und nicht zu Beginn des Spiels, damit im Menü alles stimmt
			playerList[i]->setPosition(Vector3D(0.0f, 0.0f, 0.0f));
			playerList[i]->setHeading(Vector3D(-1.0f, 0.0f, 0.0f));
			playerList[i]->setRotation(Vector3D(0.0f, 0.0f, 0.0f));
			playerList[i]->restart();	// make sure, all internal data is reset (like standUptime)
			playerList[i]->stop();
		}
		if(runServer())	{
			for(unsigned int i = 0; i < (unsigned int)playerList.size(); i++) {
				if(playerList[i]->getPlayerType() == PLAYER_TYPE_LAN)
					playerList[i]->setReadyFlag(false);
				else
					playerList[i]->setReadyFlag(true);
				sendInfoReadyFlag(i);
			}
			for(unsigned int i = 0; i < (unsigned int)playerList.size(); i++) {
				if(playerList[i]->getPlayerType() == PLAYER_TYPE_AI) {
					std::string botName = playerList[i]->getName();
					unsigned int last = (unsigned int)botName.length() - 1;
					if(botName[0] == '(' && botName[last] == ')')
						kickPlayer(i);	// Dieser Bot hat einen Spieler ersetzt -> kicken
				}
			}
		}
		int oldLanState = gameStatusFlags & GAME_STATUS_LAN_ALL;
		int oldGameType = gameStatusFlags & GAME_STATUS_TYPE_ALL;
		oldGameType |= (gameStatusFlags & GAME_STATUS_NIGHT);
		gameStatusFlags  = GAME_STATUS_NONE;
		gameStatusFlags |= GAME_STATUS_MENU;
		if(oldLanState != 0) {
			gameStatusFlags |= oldLanState;
			gameStatusFlags |= oldGameType;
            gameStatusFlags |= GAME_STATUS_LAN_WAITING;
			gameStatusFlags &= ~GAME_STATUS_LAN_READY;
			lanLobby->show();
		}
		else
			mainMenu.show();
	}
}

void MountainRanger::activateGameMode() {
	if(gameStatusFlags & GAME_STATUS_MENU) {
		mainMenu.hide();
		gameStatusFlags &= ~GAME_STATUS_MENU;
		gameStatusFlags &= ~GAME_STATUS_FINISHED;
		gameStatusFlags |=  GAME_STATUS_COUNTDOWN;
		gameStatusFlags &= ~GAME_STATUS_LAN_WAITING;
		countdown = 5.0f;
		camPlayerNum = 0;

		if(!(gameStatusFlags & GAME_STATUS_LANGAME)) {
			// Hindernisse in die Landschaft setzen
			obstacleSeed = (unsigned int)time(NULL);
			setObstacles();

			if(gameStatusFlags & GAME_STATUS_TYPE_DOWN) {
				// Modus == Downhill
#ifdef _DEBUG
				std::clog << "Starte Downhill Rennen...\n";
#endif
				addAIPlayer(NUM_AI_PLAYERS);
			}
			else if(gameStatusFlags & GAME_STATUS_TYPE_SLALOM) {
				// Modus == Slalom
#ifdef _DEBUG
				std::clog << "Starte Slalom Abfahrt...\n";
#endif
				createSlalomRoute();
			}
			else {
				// Modus == Freestyle
#ifdef _DEBUG
				std::clog << "Freestyler...\n";
#endif
				// Add some AI Players
				addAIPlayer(NUM_AI_PLAYERS);
			}
			movePlayersToStartPositions();
		}
		else if(gameStatusFlags & GAME_STATUS_SERVER) {
			obstacleSeed = (unsigned int)time(NULL);
			setObstacles();

			int aiGameMode;
			if(gameStatusFlags & GAME_STATUS_TYPE_FREE)
				aiGameMode = AI_GAME_TYPE_FREE;
			else if(gameStatusFlags & GAME_STATUS_TYPE_DOWN)
				aiGameMode = AI_GAME_TYPE_DOWN;

			for(unsigned int i = 0; i < (unsigned int)playerList.size(); i++) {
				if(playerList[i]->getPlayerType() == PLAYER_TYPE_AI)
					((AIPlayer*)playerList[i])->setGameMode(aiGameMode);
			}
			playerStartAngle = ((float)rand() / (float)RAND_MAX) * 2.0f * PI;
			movePlayersToStartPositions(playerStartAngle);

			// send all Players the start singal
			ProtStart *protStart = new ProtStart();
			protStart->giveInformation(0, obstacleSeed, playerStartAngle);
			packetProcessor->sendPacket(MAX_PLAYERS, protStart);
		}
		else {
			setObstacles();
			movePlayersToStartPositions(playerStartAngle);
		}


		// Note, that after this game, we'll get to the lobby instead of getting to the main menu:
		if(gameStatusFlags & GAME_STATUS_LANGAME) {
			//playerList[0]->setReadyFlag(false);
			//sendInfoReadyFlag(0);
			if(runServer()) {
				for(unsigned int i = 0; i < (unsigned int)playerList.size(); i++) {
					if(playerList[i]->getPlayerType() != PLAYER_TYPE_AI)
						playerList[i]->setReadyFlag(false);
					else
						playerList[i]->setReadyFlag(true);
					sendInfoReadyFlag(i);
				}
			}
			gameStatusFlags |= GAME_STATUS_LAN_RUNNING;
		}

		delete rotation;
		// Cam dreht sich zu Beginn um den Spieler
		rotation = new Vector3D(&targetRotation);
		*rotation += Vector3D(0.0f, 85.0f, 0.0f);	// 85 - 175 = -90 ==> 3rd-person-view
		targetRotation = Vector3D(0.0f, -175.0f, 0.0f);

		for(unsigned int i = 0; i < playerList.size(); i++) {
			if(playerList[i]->getPlayerType() == PLAYER_TYPE_AI)
				((AIPlayer*)playerList[i])->setNameColor(&HUDcol);
			else if(playerList[i]->getPlayerType() == PLAYER_TYPE_LAN)
				((LANPlayer*)playerList[i])->setNameColor(&HUDcol);
		}
	}
}

void MountainRanger::movePlayersToStartPositions(float startAngle) {
	unsigned int number = (unsigned int)playerList.size();
	if(number == 1) {
		// Nur ein Spieler, keine spezielle Platzierung nötig
		Vector3D diff = playerList[0]->getPosition() * -1.0f;
		playerList[0]->move(diff);
	}
	else {
		// Platziere alle Spieler gleichmäßig auf einem Kreis um den Gipfel
		float radius = number * 0.25f * PI;
		float angle  = (2.0f * PI) / number;		// Note: radians, not degrees!
		Vector3D diff = Vector3D(0.0f, 0.0f, 0.0f);
		float start;
		if(startAngle < 0.0f)
			start = ((float)rand() / (float)RAND_MAX) * 2.0f * PI;
		else
			start = startAngle;
		float myAngle;
		for(unsigned int i = 0; i < number; i++) {
			unsigned int playerNum = packetProcessor->serverPlayerNum(i);
			myAngle = start + (angle * playerNum);
			diff.x = cos(myAngle) * radius;
			diff.y = 0.0f;
			diff.z = sin(myAngle) * radius;

			diff.y = -horn->getHeightOverGround(diff);
			playerList[i]->move(diff);
			turnPlayer((-myAngle * 180.0f / PI) + 180.0f, i);
		}
	}
}

void MountainRanger::setObstacles(unsigned int number) {
#ifdef _DEBUG
			unsigned int numTrees = 0, numRocks = 0;
#endif
			srand(obstacleSeed);
			for(unsigned int i = 0; i < number; i++) {
				Actor* newObs;
				int type = rand() % OBSTACLE_DIFFERENT_TYPES;
				switch(type) {
				case 0:
					newObs = new Tree();
#ifdef _DEBUG
					numTrees++;
#endif
					break;
				case 1:
					newObs = new Rock();
#ifdef _DEBUG
					numRocks++;
#endif
					break;
				default:
					newObs = NULL;
#ifdef _DEBUG
					std::cerr << "Fehler: Hindernis-Typ " << type << " nicht angegeben!\n";
#endif
					break;
				}
				if(newObs == NULL)
					continue;

				Vector3D pos = Vector3D(0.0f, 0.0f, 0.0f);
				do {
					pos.x = ((float)rand() / RAND_MAX) * (OBSTACLE_MAX_X - OBSTACLE_MIN_X);
					pos.x += OBSTACLE_MIN_X;
				} while(pos.x > OBSTACLE_VOID_MIN_X && pos.x < OBSTACLE_VOID_MAX_X);
				do {
					pos.z = ((float)rand() / RAND_MAX) * (OBSTACLE_MAX_Z - OBSTACLE_MIN_Z);
					pos.z += OBSTACLE_MIN_Z;
				} while(pos.z > OBSTACLE_VOID_MIN_Z && pos.z < OBSTACLE_VOID_MAX_Z);
				pos.y -= horn->getHeightOverGround(pos);
				newObs->move(pos);
				obstacles.push_back(newObs);
			}
#ifdef _DEBUG
			std::clog << (numTrees + numRocks) << " Hindernisse erstellt.\n";
			std::clog << "\tdavon Baeume: " << numTrees << '\n';
			std::clog << "\tdavon Felsen: " << numRocks << '\n';
#endif
}

void MountainRanger::addAIPlayer(unsigned int number) {
	for(unsigned int i = 0; i < number; i++) {
		if(playerList.size() >= MAX_PLAYERS)
			return;
		AIPlayer* newAIPlayer = new AIPlayer();
		newAIPlayer->setLOD(GLOBAL_LEVEL_OF_DETAIL);
		playerList.push_back(newAIPlayer);
		startTime.push_back(0);
		finalTime.push_back(0);
		stylePoints* aiPoints = new stylePoints();
		style.push_back(aiPoints);
		newAIPlayer->setStylePoints(aiPoints);
		if(gameStatusFlags & GAME_STATUS_TYPE_FREE)
			newAIPlayer->setGameMode(AI_GAME_TYPE_FREE);
		else if(gameStatusFlags & GAME_STATUS_TYPE_DOWN)
			newAIPlayer->setGameMode(AI_GAME_TYPE_DOWN);

		// let all Players create Player on playerList
		ProtCreatePlayers *protCreatePlayers = new ProtCreatePlayers();
		protCreatePlayers->giveInformation(0, 1);
		packetProcessor->sendPacket(MAX_PLAYERS, protCreatePlayers);
		// send initInformation
		sendInit((unsigned int)playerList.size() - 1);
		// readyFlag
		ProtReady *packetReady = new ProtReady();
		packetReady->giveInformation((unsigned int)playerList.size() - 1, playerList[(unsigned int)playerList.size() - 1]->getReadyFlag());
		packetProcessor->sendPacket(MAX_PLAYERS, packetReady);

#ifdef _DEBUG
		std::clog << "Neuer KI-Spieler \"" << newAIPlayer->getName() << "\"\n";
#endif
		newAIPlayer->stop();
	}
}

unsigned int MountainRanger::addLanPlayer() {
    LANPlayer *playerToAdd = new LANPlayer();
    playerToAdd->setLOD(GLOBAL_LEVEL_OF_DETAIL);
    playerList.push_back(playerToAdd);
    startTime.push_back(0);
	finalTime.push_back(0);
    style.push_back(new stylePoints());
    return ((unsigned int)playerList.size() - 1);
}

void MountainRanger::renderHUD() {
	if (!gameIsRunning()) return;
	glPushMatrix();
	glLoadIdentity();

	//glColor3f(HUD_COLOR_R, HUD_COLOR_G, HUD_COLOR_B);
	HUDcol.switchToColor();
	glTranslatef(HUD_COORD_X, HUD_COORD_Y, HUD_COORD_Z);
	string text;

	GLboolean isTexEn = glIsEnabled(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	if(gameStatusFlags & GAME_STATUS_COUNTDOWN) {
		// Countdown
		glPushMatrix();
		glTranslatef(-HUD_COORD_X - 0.1f, -HUD_COORD_Y, 0.0f);
		glScalef(0.001f, 0.001f, 0.001f);
		{
			stringstream buffer;
			buffer << (int)countdown + 1;
			text.append(buffer.str());
			text.append("...");
		}	// buffer is deleted now

		for(unsigned int i = 0; i < text.length(); i++)
			glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)text[i]);
		text.erase();
		glPopMatrix();
	}
	//else if(!(gameStatusFlags & GAME_STATUS_FINISHED)) {
	else if(!(playerList[camPlayerNum]->isStopped() ||
		(camPlayerNum == 0 && gameStatusFlags & GAME_STATUS_FINISHED))) {
		// Speedometer
		{
			float currentSpeed = playerList[camPlayerNum]->getSpeed();
			glPushMatrix();
			glScalef(0.0005f, 0.0005f, 0.0005f);
			{
				stringstream buffer;
				buffer << (int)(3.6f * currentSpeed);
				text.append("Speed: ");
				text.append(buffer.str());
				text.append("km/h");
			}	// buffer is deleted now

			for(unsigned int i = 0; i < text.length(); i++)
				glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)text[i]);
			text.erase();
			glPopMatrix();

			// Render graphical speedometer
			glPushMatrix();
			glTranslatef(0.2f, -0.25f, 0.0f);
			glBegin(GL_LINE_STRIP);
			for(unsigned int i = 0; i < GLOBAL_LEVEL_OF_DETAIL; i++) {
				float angle = (i / (GLOBAL_LEVEL_OF_DETAIL - 1)) * PI;
				float x = 0.2f * cos(angle);
				float y = 0.2f * sin(angle);
				glVertex3f(x, y, 0.0f);
			}
			glEnd();
			glRotatef(-180.0f * currentSpeed / 37.5f, 0.0f, 0.0f, 1.0f);	// 37.5 m/s == 135 km/h
			float g = (currentSpeed / 37.5f) * 0.8f;
			float r = 0.8f - g;
			glColor3f(r, g, 0.0f);
			glBegin(GL_LINES);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(-0.19f, 0.0f, 0.0f);
			glEnd();
			HUDcol.switchToColor();
			glPopMatrix();
		}

		// flying indicator
		if(playerList[camPlayerNum]->isFlying()) {
			glPushMatrix();
			glTranslatef(0.8f, 0.0f, 0.0f);
			glScalef(0.0005f, 0.0005f, 0.0005f);
			text.append("Flying!");
			for(unsigned int i = 0; i < text.length(); i++)
				glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)text[i]);
			text.erase();
			glPopMatrix();
		}

		// jump bonus indicator
		{
			float currentJumpBonus = playerList[camPlayerNum]->getCurrentJumpBonus();
			glPushMatrix();
			glTranslatef(0.0f, -1.9f, 0.0f);
			//text = "Sprungbonus:";
			text = "Turbo Boost:";
			glPushMatrix();
			glScalef(0.0006f, 0.0006f, 0.0006f);
			for(unsigned int i = 0; i < text.length(); i++)
				glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)text[i]);
			glPopMatrix();
			text.erase();

			glTranslatef(0.0f, -0.1f, 0.0f);
			float r, g;
			g = currentJumpBonus * 0.8f;
			r = (1 - currentJumpBonus) * 0.8f;
			glColor3f(r, g, 0.0f);
			glBegin(GL_TRIANGLE_FAN);
			glVertex3f(0.0f, 0.0f,  0.0f);
			glVertex3f(0.4f * currentJumpBonus, 0.0f,  0.0f);
			glVertex3f(0.4f * currentJumpBonus, 0.05f, 0.0f);
			glVertex3f(0.0f, 0.05f, 0.0f);
			glEnd();

			HUDcol.switchToColor();
			glBegin(GL_LINE_STRIP);
			glVertex3f(0.0f, 0.0f,  0.0f);
			glVertex3f(0.4f, 0.0f,  0.0f);
			glVertex3f(0.4f, 0.05f, 0.0f);
			glVertex3f(0.0f, 0.05f, 0.0f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glEnd();

			if(playerList[camPlayerNum]->isFlying()) {
				std::string comment = "";

				if(currentJumpBonus >= 0.95f) {
					comment = "Mo-Mo-Mo-Monster Jump!!";
					if(gameStatusFlags & GAME_STATUS_TYPE_FREE) {
						stringstream buffer;
						buffer << MONSTER_JUMP_POINTS;
						comment.append(" (+");
						comment.append(buffer.str());
						comment.append(")");
					}
				}
				else if(currentJumpBonus >= 0.7f)
					comment = "Ultra Jump!";
				else if(currentJumpBonus >= 0.5f)
					comment = "Good Jump";
				else if(currentJumpBonus >= 0.1f)
					comment = "...netter Versuch";

				glTranslatef(0.5f, 0.0f, 0.0f);
				glPushMatrix();
				glScalef(0.0006f, 0.0006f, 0.0006f);
				for(unsigned int i = 0; i < comment.length(); i++)
					glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)comment[i]);
				glPopMatrix();
			}
			glPopMatrix();
		}

		// height indicator
		{
			float heightFactor = horn->getHeightOverGround(playerList[camPlayerNum]->getPosition());
			float localPlayerMaxHeight = style[camPlayerNum]->getMaxHeight();

			glPushMatrix();
			glTranslatef(2.3f, -2.0f, 0.0f);
			stringstream bufferCur, bufferMax;
			bufferCur << (int)heightFactor;
			bufferMax << (int)localPlayerMaxHeight;

			text = "Hoehe: ";
			text.append(bufferCur.str());
			text.append("m");
			glPushMatrix();
			glScalef(0.0005f, 0.0005f, 0.0005f);
			for(unsigned int i = 0; i < text.length(); i++)
				glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)text[i]);
			glPopMatrix();
			text.erase();

			float maxHeightFactor;
			heightFactor /= 100.0f;
			if(heightFactor < 0.0f)
				heightFactor = 0.0f;
			heightFactor += 0.01f;
			maxHeightFactor = localPlayerMaxHeight / 100.0f;

			if(localPlayerMaxHeight >= 10.0f) {
				if(localPlayerMaxHeight >= stylePoints::minHeightForPoints)
					glColor3f(0.0f, 0.8f, 0.0f);
				else if(localPlayerMaxHeight >= (stylePoints::minHeightForPoints / 2.0f))
					glColor3f(0.8f, 0.8f, 0.0f);
				else
					glColor3f(1.0f, 0.0f, 0.0f);
				text.append(bufferMax.str());
				text.append("m");
				glPushMatrix();
				float textMaxHeightFactor = maxHeightFactor;
				if(textMaxHeightFactor > 2.0f)
					textMaxHeightFactor = 2.0f;
				glTranslatef(0.25f, textMaxHeightFactor, 0.0f);
				glScalef(0.0005f, 0.0005f, 0.0005f);
				for(unsigned int i = 0; i < text.length(); i++)
					glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)text[i]);
				glPopMatrix();
				text.erase();
				glColor3f(HUD_COLOR_R, HUD_COLOR_G, HUD_COLOR_B);
			}

			glTranslatef(0.5f, 0.0f, 0.0f);
			glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(0.0f,  0.0f, 0.0f);
			glVertex3f(0.08f, 0.0f, 0.0f);
			glVertex3f(0.0f,  heightFactor, 0.0f);
			glVertex3f(0.08f, heightFactor, 0.0f);
			glEnd();

			glColor3f(0.8f, 0.0f, 0.0f);
			glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(0.0f,  maxHeightFactor, 0.0f);
			glVertex3f(0.08f, maxHeightFactor, 0.0f);
			glVertex3f(0.0f,  maxHeightFactor + 0.01f, 0.0f);
			glVertex3f(0.08f, maxHeightFactor + 0.01f, 0.0f);
			HUDcol.switchToColor();
			glEnd();
			glPopMatrix();
		}

		// compass
		{
			// -z axis: north
			Vector3D dir = playerList[camPlayerNum]->getHeading();
			dir.y = 0.0f;
			float direction = dir.getAngle(Vector3D(0.0f, 0.0f, -1.0f));
			if(dir.getAngle(Vector3D(-1.0f, 0.0f, 0.0f)) < 90.0f)
				direction = 360.0f - direction;		// in western direction...

			text = "N  l l | l l O  l l | l l S  l l | l l W  l l | l l ";
			/* Note:
				* small "L" for regular 15°-marks
				* pipe | for 45°-marks
				* capital letters ("N", "S", "W" & "O") for 90°-marks
				one space between each symbol
			*/
			glPushMatrix();
			glTranslatef(-HUD_COORD_X, 0.1f, 0.0f);
			float angle = 360.0f / (float)text.length();
			for(unsigned int i = 0; i < text.length(); i++) {
				float myAngle = (-angle * i) + direction;
				if(myAngle > 360.0f)
					myAngle -= 360.0f;
				if(myAngle < 0.0f)
					myAngle += 360.0f;
				if(myAngle > 90.0f && myAngle < 270.0f)
					continue;
				glPushMatrix();
				glRotatef(myAngle, 0.0f, 1.0f, 0.0f);
				glTranslatef(0.0f, 0.0f, -0.2f);
				glScalef(0.0005f, 0.0005f, 0.0005f);
				glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)text[i]);
				glPopMatrix();
			}
			text.erase();
			glPopMatrix();
		}
		// time elapsed
		if(startTime[camPlayerNum] > 0) {
			glPushMatrix();
			glTranslatef(0.0f, -0.40f, 0.0f);
			glScalef(0.0005f, 0.0005f, 0.0005f);

			float runTime = (float)(clock() - startTime[camPlayerNum]);
			runTime /= CLK_TCK;
			if(gameStatusFlags & GAME_STATUS_TYPE_FREE)
				runTime = FREESTYLE_DURATION - runTime;	// Countdown in Freestyle-Mode
			int minutes = 0;
			while(runTime > 60.0f) {
				minutes++;
				runTime -= 60.0f;
			}
			stringstream buffer;
			buffer << "Zeit: ";
			if(minutes) {
				buffer << minutes << ":";
				if(runTime < 10.0f)
					buffer << "0";		// führende 0
			}
			buffer.precision(2);
			buffer << fixed << runTime;
			text.append(buffer.str());

			for(unsigned int i = 0; i < text.length(); i++)
				glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)text[i]);
			text.erase();
			glPopMatrix();
		}
		// Style-Points
		if(gameStatusFlags & GAME_STATUS_TYPE_FREE) {
			// Total Points
			glPushMatrix();
			glTranslatef(0.0f, -0.50f, 0.0f);
			glScalef(0.0005f, 0.0005f, 0.0005f);
			{
				stringstream buffer;
				buffer << "Style Points: ";
				buffer << style[camPlayerNum]->getTotalPoints();
				text.append(buffer.str());
			}
			for(unsigned int i = 0; i < text.length(); i++)
				glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)text[i]);
			text.erase();
			glPopMatrix();

			// current points & multiplicator
			if(style[camPlayerNum]->isActive() != stylePoints::NOT_ACTIVE) {
				glPushMatrix();
				glTranslatef(0.0f, -0.65f, 0.0f);
				glScalef(0.0008f, 0.0008f, 0.0008f);
				{
					text = "     ";
					stringstream buffer;
					int points = style[camPlayerNum]->getCurrentPoints();
					if(points < 0)
						glColor3f(0.8f, 0.0f, 0.0f);

					buffer << showpos << points << noshowpos;
					if(style[camPlayerNum]->isActive() == stylePoints::ACTIVE_COUNTING) {
						int mult = style[camPlayerNum]->getCurrentMultiplicator();
						if(mult > 1)
							buffer << " x" << mult;
					}
					text.append(buffer.str());
				}
				for(unsigned int i = 0; i < text.length(); i++)
					glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)text[i]);
				HUDcol.switchToColor();
				text.erase();
				glPopMatrix();
			}
		}

		// downhill progress
		if(gameStatusFlags & GAME_STATUS_TYPE_DOWN) {
			stringstream buffer;
			buffer << "Fortschritt: ";
			float pos = playerList[camPlayerNum]->getPosition().y;
			float percent = ((pos / TRACK_BORDER) * 100.0f);
			if (percent < 0.0f)
				percent = 0.0f;
			if (percent > 100.0f)
				percent = 100.0f;
			buffer << (int)percent << '%';
			text = buffer.str();

			glPushMatrix();
			glTranslatef(0.0f, -0.5f, 0.0f);
			glScalef(0.0006f, 0.0006f, 0.0006f);
			for(unsigned int i = 0; i < text.length(); i++)
				glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)text[i]);
			text.erase();
			glPopMatrix();
		}

		// time-out for next slalom gate & current gate-#
		if((gameStatusFlags & GAME_STATUS_TYPE_SLALOM) && !(gameStatusFlags & GAME_STATUS_COUNTDOWN)) {
			// Time-Out
			glPushMatrix();
			glTranslatef(0.0f, -0.50f, 0.0f);
			glScalef(0.0005f, 0.0005f, 0.0005f);
			{
				stringstream buffer;
				buffer << "Gate Time-Out: ";
				buffer.precision(2);
				buffer << fixed << gates[currentGate]->getTimeOut();
				text.append(buffer.str());
			}
			for(unsigned int i = 0; i < text.length(); i++)
				glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)text[i]);
			text.erase();
			glPopMatrix();

			// Gate-#
			glPushMatrix();
			glTranslatef(0.0f, -0.60f, 0.0f);
			glScalef(0.0005f, 0.0005f, 0.0005f);
			{
				stringstream buffer;
				buffer << "aktuelles Gate: ";
				buffer << (currentGate + 1) << "/" << (unsigned int)gates.size();
				text.append(buffer.str());
			}
			for(unsigned int i = 0; i < text.length(); i++)
				glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)text[i]);
			text.erase();
			glPopMatrix();
		}
		// points of all players (list)
		if(gameStatusFlags & GAME_STATUS_TYPE_FREE) {
			unsigned int styleSize = (unsigned int)style.size();
#ifdef _DEBUG
			_ASSERT(styleSize == playerList.size());
#endif
			long* ranking = new long[styleSize];
			for(unsigned int i = 0; i < styleSize; i++)
				ranking[i] = style[i]->getTotalPoints();
			qsort(ranking, styleSize, sizeof(long), compareFuncLong);
			//unsigned int samePointsCounter = 0;
			for(unsigned int i = 0; i < styleSize; i++) {
				{
					stringstream buffer;
					buffer << playerList[i]->getName() << ": ";
					buffer << style[i]->getTotalPoints();
					text = buffer.str();
				}
				unsigned int loc = 0;
				while(style[i]->getTotalPoints() != ranking[loc])
					loc++;
				ranking[loc] = -1;

				glPushMatrix();
				glTranslatef(2.0f, -0.08f * loc, 0.0f);
				if(i == 0) {
					glPushMatrix();
					glTranslatef(-0.05f, 0.02f, 0.0f);
					glColor3f(0.0f, 0.0f, 1.0f);
					glutSolidSphere(0.02, (int)GLOBAL_LEVEL_OF_DETAIL, (int)GLOBAL_LEVEL_OF_DETAIL);
					HUDcol.switchToColor();
					glPopMatrix();
				}
				glScalef(0.0004f, 0.0004f, 0.0004f);
				for(unsigned int c = 0; c < text.length(); c++)
					glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)text[c]);
				glPopMatrix();
			}
			delete ranking;
			text.erase();
		}
		// progress of all players (list)
		if(gameStatusFlags & GAME_STATUS_TYPE_DOWN) {
			unsigned int plSize = (unsigned int)playerList.size();
			unsigned int alreadyStopped = 0;
			long *rankingStopped = new long[plSize];
			for (unsigned int i = 0; i < plSize; i++) {
				if (playerList[i]->isStopped()) {
					alreadyStopped++;
					rankingStopped[i] = (long)(finalTime[i] * -100.0f);
				} else {
					rankingStopped[i] = -9999999;
				}
			}
			qsort(rankingStopped, plSize, sizeof(long), compareFuncLong);
			for (unsigned int i = 0; i < plSize; i++) {
				if (!playerList[i]->isStopped()) continue;
				unsigned int loc = 0;
				while((long)(finalTime[i] * -100.0f) != rankingStopped[loc])
					loc++;
				rankingStopped[loc] = -9999999;
				stringstream buffer;
				buffer << playerList[i]->getName() << ": ";
				buffer.precision(1);
				double runTime = finalTime[i];
				int minutes = 0;
				while(runTime > 60.0) {
					minutes++;
					runTime -= 60.0;
				}
				if(minutes) {
					buffer << minutes << ":";
					if(runTime < 10.0)
						buffer << "0";		// führende 0
				}
				buffer.precision(2);
				buffer << fixed << runTime;
				text = buffer.str();
				
				glPushMatrix();
				glTranslatef(2.0f, -0.08f * loc, 0.0f);
				glScalef(0.0004f, 0.0004f, 0.0004f);
				for(unsigned int c = 0; c < text.length(); c++)
					glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)text[c]);
				glPopMatrix();
			}
			delete rankingStopped;
			text.erase();

			// display not stopped
			long* ranking = new long[plSize];
			for(unsigned int i = 0; i < plSize; i++) {
				if (!playerList[i]->isStopped()) {
                    ranking[i] = (long)(-10.0f * playerList[i]->getPosition().y);
				} else {
					ranking[i] = -1;
				}
			}
			qsort(ranking, plSize, sizeof(long), compareFuncLong);
			for(unsigned int i = 0; i < plSize; i++) {
				if (playerList[i]->isStopped()) continue;
				{
					stringstream buffer;
					buffer << playerList[i]->getName() << ": ";
					buffer.precision(1);
					float pos = playerList[i]->getPosition().y;
					float percent = ((pos / TRACK_BORDER) * 100.0f);
					if (percent < 0.0f) percent = 0.0f;
					if (percent > 100.0f) percent = 100.0f;
					buffer << (int)percent << '%';
					text = buffer.str();
				}
				unsigned int loc = 0;
				while((long)(-10.0f * playerList[i]->getPosition().y) != ranking[loc])
					loc++;
				ranking[loc] = -1;
				loc += alreadyStopped;

				glPushMatrix();
				glTranslatef(2.0f, -0.08f * loc, 0.0f);
				if(i == 0) {
					glPushMatrix();
					glTranslatef(-0.05f, 0.02f, 0.0f);
					glColor3f(0.0f, 0.0f, 1.0f);
					glutSolidSphere(0.02, (int)GLOBAL_LEVEL_OF_DETAIL, (int)GLOBAL_LEVEL_OF_DETAIL);
					HUDcol.switchToColor();
					glPopMatrix();
				}
				glScalef(0.0004f, 0.0004f, 0.0004f);
				for(unsigned int c = 0; c < text.length(); c++)
					glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)text[c]);
				glPopMatrix();
			}
			delete ranking;
			text.erase();
		}
	}
	else {
		// End-of-game screen
		glPushMatrix();
		glTranslatef(0.8f, -0.3f, 0.0f);
		glScalef(0.001f, 0.001f, 0.001f);
		text.append("Spielende");
		for(unsigned int i = 0; i < text.length(); i++)
			glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)text[i]);
		text.erase();
		glPopMatrix();

		// Spieltypabhängige Endwerte
		glPushMatrix();
		glTranslatef(0.2f, -0.5f, 0.0f);
		if(gameStatusFlags & GAME_STATUS_TYPE_DOWN) {
			// Downhill
			text = "Zeit:";

			glPushMatrix();
			glScalef(0.0008f, 0.0008f, 0.0008f);
			for(unsigned int i = 0; i < text.length(); i++)
				glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)text[i]);
			text.erase();
			glPopMatrix();

			double* ranking = new double[playerList.size()];
			for(unsigned int i = 0; i < playerList.size(); i++)
				ranking[i] = finalTime[i];
			qsort(ranking, playerList.size(), sizeof(double), compareFuncDouble);
			unsigned int samePointsCounter = 0;
			unsigned int loc;
			for(unsigned int n = 0; n < playerList.size(); n++) {
				double runTime = finalTime[n];
				if(runTime > 0) {
					loc = 0;
					while(runTime != ranking[loc])
						loc++;
					ranking[loc] = -1.0;

					if(loc == 0 && runTime < hsDownhill.value) {
						// update Highscore
						hsDownhill.value = finalTime[n];
						hsDownhill.name  = playerList[n]->getName();
						configWasChanged = true;
#ifdef _DEBUG
					std::clog << "Neuere Highscore gefunden, aktualisiere\n";
#endif
					}

					int minutes = 0;
					while(runTime > 60.0f) {
						minutes++;
						runTime -= 60.0f;
					}
					stringstream runTimeStream;
					runTimeStream << minutes << ":";
					if(runTime < 10.0f)
						runTimeStream << "0";
					runTimeStream.precision(2);
					runTimeStream << fixed << runTime;
					text.append(playerList[n]->getName());
					text.append(": ");
					text.append(runTimeStream.str());

					glPushMatrix();
					if(loc >= 8) {
						glTranslatef(-MENU_COORD_X, 0.0f, 0.0f);
						loc -= 8;
					}
					glTranslatef(0.0f, -((loc+1)*0.15f), 0.0f);
					if((gameStatusFlags & GAME_STATUS_LANGAME) && playerList[n]->isStopped()) {
						// visualize ready-flag
						if(playerList[n]->getReadyFlag())
							glColor3f(0.0f, 1.0f, 0.0f);
						else
							glColor3f(1.0f, 0.0f, 0.0f);
						glEnable(GL_LIGHTING);
						glPushMatrix();
						glTranslatef(-0.15f, 0.04f, 0.0f);
						glutSolidSphere(0.05, (int)GLOBAL_LEVEL_OF_DETAIL, (int)GLOBAL_LEVEL_OF_DETAIL);
						glPopMatrix();
						HUDcol.switchToColor();
						glDisable(GL_LIGHTING);
					}
					glScalef(0.0008f, 0.0008f, 0.0008f);
					for(unsigned int i = 0; i < text.length(); i++)
						glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)text[i]);
					text.erase();
					glPopMatrix();
				}
			}
			delete ranking;
			// display Highscore
			{
				glPushMatrix();
				glColor3f(0.0f, 0.4f, 0.0f);
				glTranslatef(0.9f, 0.0f, 0.0f);
				glScalef(0.0008f, 0.0009f, 0.0008f);
				float runTime = (float)hsDownhill.value;
				int minutes = 0;
				while(runTime > 60.0f) {
					minutes++;
					runTime -= 60.0f;
				}
				stringstream runTimeStream;
				runTimeStream << minutes << ":";
				if(runTime < 10.0f)
					runTimeStream << "0";
				runTimeStream.precision(2);
				runTimeStream << fixed << runTime;
				text.append("Rekord: ");
				text.append(runTimeStream.str());
				text.append(" von ");
				text.append(hsDownhill.name);
				for(unsigned int i = 0; i < text.length(); i++) {
					glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)text[i]);
				}
				text.erase();
				glPopMatrix();
			}
		}
		else if(gameStatusFlags & GAME_STATUS_TYPE_SLALOM) {
			// Slalom
			glPushMatrix();
			text = "verpasste Gates: ";
			int missedGates = 0;
			{
				stringstream buffer;
				for(unsigned int i = 0; i < gates.size(); i++) {
					if(gates[i]->isActive() & SLALOM_GATE_TIMED_OUT)
						missedGates++;
				}
				buffer << missedGates;
				text.append(buffer.str());
			}
			glScalef(0.0008f, 0.0008f, 0.0008f);
			for(unsigned int i = 0; i < text.length(); i++)
				glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)text[i]);
			text.erase();
			glPopMatrix();
			glTranslatef(0.0f, -0.15f, 0.0f);

			glPushMatrix();
			float timePenalty = SLALOM_GATE_MISS_PENALTY * missedGates;
			if(missedGates > 0) {
				glPushMatrix();
				text = "Strafzeit: ";
				stringstream buffer;
				buffer << timePenalty;
				text.append(buffer.str());
				text.append("s");
				glScalef(0.0008f, 0.0008f, 0.0008f);
				for(unsigned int i = 0; i < text.length(); i++)
					glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)text[i]);
				text.erase();
				glPopMatrix();
				glTranslatef(0.0f, -0.15f, 0.0f);
			}

			float runTime = (float)finalTime[0];
			int minutes = 0;
			while(runTime > 60.0f) {
				minutes++;
				runTime -= 60.0f;
			}
			stringstream runTimeStream;
			runTimeStream << minutes << ":";
			if(runTime < 10.0f)
				runTimeStream << "0";
			runTimeStream.precision(2);
			runTimeStream << fixed << runTime;

			text = "Gesamtzeit: ";
			text.append(runTimeStream.str());

			glScalef(0.0008f, 0.0008f, 0.0008f);
			for(unsigned int i = 0; i < text.length(); i++)
				glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)text[i]);
			text.erase();
			glPopMatrix();

			// display Highscore
			{
				glPushMatrix();
				glColor3f(0.0f, 0.4f, 0.0f);
				glTranslatef(0.0f, -0.45f, 0.0f);
				glScalef(0.0008f, 0.0009f, 0.0008f);
				float runTime = (float)hsSlalom.value;
				int minutes = 0;
				while(runTime > 60.0f) {
					minutes++;
					runTime -= 60.0f;
				}
				stringstream runTimeStream;
				runTimeStream << minutes << ":";
				if(runTime < 10.0f)
					runTimeStream << "0";
				runTimeStream.precision(2);
				runTimeStream << fixed << runTime;
				text.append("Rekord: ");
				text.append(runTimeStream.str());
				for(unsigned int i = 0; i < text.length(); i++)
					glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)text[i]);
				text.erase();
				glPopMatrix();
			}

		}
		else {
			// Freestyle
			text = "Style-Points:";
			//text.append(" [n/a]");

			glPushMatrix();
			glScalef(0.0008f, 0.0008f, 0.0008f);
			for(unsigned int i = 0; i < text.length(); i++)
				glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)text[i]);
			text.erase();
			glPopMatrix();

			long* ranking = new long[playerList.size()];
			for(unsigned int i = 0; i < playerList.size(); i++)
				ranking[i] = style[i]->getTotalPoints();
			qsort(ranking, playerList.size(), sizeof(long), compareFuncLong);
			unsigned int samePointsCounter = 0;
			unsigned int loc;
			for(unsigned int n = 0; n < playerList.size(); n++) {
				stringstream buffer;
				buffer << style[n]->getTotalPoints();
				text = playerList[n]->getName();
				text.append(": ");
				if(!playerList[n]->isStopped())
					text.append("(");
				text.append(buffer.str());
				if(!playerList[n]->isStopped())
					text.append(")");

				loc = 0;
				while(style[n]->getTotalPoints() != ranking[loc])
					loc++;
				ranking[loc] = -1;
				if(loc == 0 && style[n]->getTotalPoints() > hsFreestyle.value) {
					// update Highscore
					hsFreestyle.value = style[n]->getTotalPoints();
					hsFreestyle.name  = playerList[n]->getName();
					configWasChanged = true;
#ifdef _DEBUG
					std::clog << "Neuere Highscore gefunden, aktualisiere\n";
#endif
				}

				glPushMatrix();
				if(loc >= 8) {
					glTranslatef(-MENU_COORD_X, 0.0f, 0.0f);
					loc -= 8;
				}
				glTranslatef(0.0f, -0.15f * (loc+1), 0.0f);
				if((gameStatusFlags & GAME_STATUS_LANGAME) && playerList[n]->isStopped()) {
					// visualize ready-flag
					if(playerList[n]->getReadyFlag())
						glColor3f(0.0f, 1.0f, 0.0f);
					else
						glColor3f(1.0f, 0.0f, 0.0f);
					glEnable(GL_LIGHTING);
					glPushMatrix();
					glTranslatef(-0.15f, 0.04f, 0.0f);
					glutSolidSphere(0.05, (int)GLOBAL_LEVEL_OF_DETAIL, (int)GLOBAL_LEVEL_OF_DETAIL);
					glPopMatrix();
					HUDcol.switchToColor();
					glDisable(GL_LIGHTING);
				}
				glScalef(0.0008f, 0.0008f, 0.0008f);
				for(unsigned int i = 0; i < text.length(); i++)
					glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)text[i]);
				text.erase();
				glPopMatrix();
			}
			delete ranking;
			// display Highscore
			{
				glPushMatrix();
				glColor3f(0.0f, 0.4f, 0.0f);
				glTranslatef(0.9f, 0.0f, 0.0f);
				glScalef(0.0008f, 0.0009f, 0.0008f);
				stringstream buffer;
				buffer << hsFreestyle.value;
				text.append("Rekord: ");
				text.append(buffer.str());
				text.append(" von ");
				text.append(hsFreestyle.name);
				for(unsigned int i = 0; i < text.length(); i++) {
					glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)text[i]);
				}
				text.erase();
				glPopMatrix();
			}
		}

		if(gameStatusFlags & GAME_STATUS_LANGAME) {
			//std::string pleaseWait = "...bitte auf andere Spieler warten...";
#define _TEXT_PLEASE_WAIT "...bitte auf andere Spieler warten..."
			text = "...bitte Enter druecken...";
			if(runServer()) {
				if(!networkAllPlayersReady())
					text = _TEXT_PLEASE_WAIT;
			}
			else if(playerList[0]->getReadyFlag())
				text = _TEXT_PLEASE_WAIT;

			HUDcol.switchToColor();
			glPushMatrix();
			glTranslatef(0.3f, -1.4f, 0.0f);
			glScalef(0.0008f, 0.0008f, 0.0008f);
			for(unsigned int i = 0; i < text.length(); i++)
				glutStrokeCharacter(GLUT_STROKE_ROMAN, (int)text[i]);
			text.erase();
			glPopMatrix();
		}
        glPopMatrix();
	}

	glDisable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	if(isTexEn) glEnable(GL_TEXTURE_2D);
	glPopMatrix();
}

void MountainRanger::buildMainMenu() {
	menu *mNewGame, *mLanGame, *mOptions, *mExit;
	menu *mLanJoin;
	lobbyMenu	*mLanLobby;
	menu *mOptSpieler, *mOptVideo, *mOptCredits, *mOptHigh, *mOptClrHS;
	std::string buffer;

	mainMenu = menu();
	mNewGame	= new menu(&mainMenu);
	mLanGame	= new menu(&mainMenu);
	mOptions	= new menu(&mainMenu);
	mExit		= new menu(&mainMenu);

	mLanJoin	= new menu(mLanGame);
	mLanLobby	= new lobbyMenu(mLanGame);
	mLanLobby->setPlayerList(&playerList);
	lanLobby = mLanLobby;

	mOptSpieler	= new menu(mOptions);
	mOptVideo	= new menu(mOptions);
	mOptCredits	= new credMenu(mOptions);
	mOptHigh	= new menu(mOptions);
	mOptClrHS	= new menu(mOptHigh);

	// +++ NEW GAME +++
	mNewGame->addEntry("Freestyle");
	mNewGame->addEntry("Downhill");
	mNewGame->addEntry("Slalom");
	mNewGame->addEntry("Uhrzeit: ???");
	mNewGame->addEntry("Abbrechen");
	mNewGame->callbackFunc(menuCB);

	// +++ LAN GAME +++
	mLanLobby->setTitle(MENU_TITLE_LAN_LOBBY);
	mLanLobby->callbackFunc(menuCB);

	buffer = "IP-Adresse: ";
	buffer.append(IPAddress);
	mLanJoin->addEntry(buffer);
	mLanJoin->addSubMenu("Verbinden", mLanLobby);
	mLanJoin->addEntry("Abbrechen");
	mLanJoin->callbackFunc(menuCB);

	mLanGame->addSubMenu(MENU_TITLE_LAN_CREATE, mLanLobby);
	mLanGame->addSubMenu(MENU_TITLE_LAN_JOIN, mLanJoin);
	mLanGame->addEntry("Abbrechen");
	mLanGame->callbackFunc(menuCB);

	// +++ OPTIONS +++
	buffer = "Spieler: ";
	if(playerList[0]->myChar->getType() == AVATAR_TYPE_BOARDER)
		buffer.append("Boarder");
	else
		buffer.append("Carver");
	mOptSpieler->addEntry(buffer);
	mOptSpieler->addEntry("Haarfarbe");
	mOptSpieler->addEntry("Hautfarbe");
	mOptSpieler->addEntry("Jacke");
	mOptSpieler->addEntry("Handschuhe");
	mOptSpieler->addEntry("Hose");
	mOptSpieler->addEntry("Schuhe");
	mOptSpieler->addEntry("Kamera: HeadCam");
	buffer = "Name: ";
	buffer.append(playerList[0]->getName());
	mOptSpieler->addEntry(buffer);
	mOptSpieler->addEntry("Fertig");
	mOptSpieler->callbackFunc(menuCB);

	mOptVideo->addEntry("Bildschirm: ???");
	//mOptVideo->addEntry("Vollbild: kA");
	mOptVideo->addEntry("Widescreen: ???");
	mOptVideo->addEntry("Fertig");
	mOptVideo->callbackFunc(menuCB);

	mOptCredits->addEntry("Programmierung:");
	mOptCredits->addEntry("  Florian Krueger");
	mOptCredits->addEntry("  Ralf Stauder");
	mOptCredits->addEntry("besonderer Dank an:");
	mOptCredits->addEntry("  Martin Groher");
	mOptCredits->addEntry("  Joerg Traub");
	mOptCredits->addEntry("  Tobias Sielhorst");
	mOptCredits->setTitle(MENU_TITLE_CREDITS);
	mOptCredits->callbackFunc(menuCB);

	{
		stringstream temp;
		temp << "Freestyle: " << hsFreestyle.name << " - " << hsFreestyle.value << " SP";
		buffer = temp.str();
	}
	mOptHigh->addEntry(buffer);
	{
		stringstream temp;
		double seconds = hsDownhill.value;
		int min = 0;
		while(seconds >= 60.0) {
			min++;
			seconds -= 60.0;
		}
		temp << "Downhill: " << hsDownhill.name << " - " << min << ":";
		if(seconds < 10.0)
			temp << "0";
		temp.precision(2);
		temp << fixed << seconds;
		buffer = temp.str();
	}
	mOptHigh->addEntry(buffer);
	{
		stringstream temp;
		double seconds = hsSlalom.value;
		int min = 0;
		while(seconds >= 60.0) {
			min++;
			seconds -= 60.0;
		}
		temp << "Slalom: " << hsSlalom.name << " - " << min << ":";
		if(seconds < 10.0)
			temp << "0";
		temp.precision(2);
		temp << fixed << seconds;
		buffer = temp.str();
	}
	mOptHigh->addEntry(buffer);
	mOptHigh->addSubMenu(MENU_TITLE_CLEAR_HS, mOptClrHS);
	mOptHigh->addEntry("Fertig");
	mOptHigh->callbackFunc(menuCB);

	mOptClrHS->addEntry("Abbrechen");
	mOptClrHS->addEntry("OK");
	mOptClrHS->callbackFunc(menuCB);

	mOptions->addSubMenu(MENU_TITLE_OPT_PLAYER,	mOptSpieler);
	mOptions->addSubMenu(MENU_TITLE_OPT_VIDEO,	mOptVideo);
	mOptions->addSubMenu(MENU_TITLE_CREDITS, mOptCredits);
	mOptions->addSubMenu(MENU_TITLE_HIGHSCORE, mOptHigh);
	mOptions->addEntry("Fertig");
	mOptions->callbackFunc(menuCB);

	// +++ EXIT +++
	mExit->setTitle(MENU_TITLE_EXIT);
	mExit->addEntry("Nein");
	mExit->addEntry("Ja");
	mExit->callbackFunc(menuCB);

	// +++ MAIN MENU +++
	mainMenu.setTitle(MENU_TITLE_MAIN);
	mainMenu.addSubMenu(MENU_TITLE_NEWGAME, mNewGame);
	mainMenu.addSubMenu(MENU_TITLE_LANGAME, mLanGame);
	mainMenu.addSubMenu(MENU_TITLE_OPTIONS, mOptions);
	mainMenu.addSubMenu("Ende", mExit);
	mainMenu.callbackFunc(menuCB);
	mainMenu.setColors(Color(0.0f, 0.7f, 0.2f), Color(1.0f, 0.0f, 0.0f), Color(0.0f, 0.0f, 1.0f));
}

void MountainRanger::menuCallback(menu* calledMenu, unsigned int index) {
	std::string currentMenu = calledMenu->getTitle();
#ifdef _DEBUG
	std::cout << "Callback from Menu: " << currentMenu << "\tIndex: " << index << "\n";
#endif

	if(currentMenu == MENU_TITLE_MAIN) {
		// Main Menu
		switch(index) {
		case 0:
			{
				setNightFlag();
				std::string uhrzeit = "Uhrzeit: ";
				if(gameStatusFlags & GAME_STATUS_NIGHT)
					uhrzeit.append("Nacht");
				else
					uhrzeit.append("Tag");
				mainMenu.getCurrentMenu()->getEntry(3)->setLabel(uhrzeit);
			}
			break;
		default:
			break;
		}
	}
	else if(currentMenu == MENU_TITLE_NEWGAME) {
		// Neues Spiel
		switch(index) {
		case 0:
			gameStatusFlags |=  GAME_STATUS_TYPE_FREE;
			gameStatusFlags &= ~GAME_STATUS_TYPE_DOWN;
			gameStatusFlags &= ~GAME_STATUS_TYPE_SLALOM;
			gameStatusFlags &= ~GAME_STATUS_LANGAME;
			activateGameMode();
			break;
		case 1:
			gameStatusFlags &= ~GAME_STATUS_TYPE_FREE;
			gameStatusFlags |=  GAME_STATUS_TYPE_DOWN;
			gameStatusFlags &= ~GAME_STATUS_TYPE_SLALOM;
			gameStatusFlags &= ~GAME_STATUS_LANGAME;
			activateGameMode();
			break;
		case 2:
			gameStatusFlags &= ~GAME_STATUS_TYPE_FREE;
			gameStatusFlags &= ~GAME_STATUS_TYPE_DOWN;
			gameStatusFlags |=  GAME_STATUS_TYPE_SLALOM;
			gameStatusFlags &= ~GAME_STATUS_LANGAME;
			activateGameMode();
			break;
		case 3:
			// Uhrzeit
			if(gameStatusFlags & GAME_STATUS_NIGHT) {
				gameStatusFlags &= ~GAME_STATUS_NIGHT;
				calledMenu->getCurrentEntry()->setLabel("Uhrzeit: Tag");
			}
			else {
				gameStatusFlags |= GAME_STATUS_NIGHT;
				calledMenu->getCurrentEntry()->setLabel("Uhrzeit: Nacht");
			}
			updateLigthing();
			break;
		case 4:
			// Abbrechen
			setNightFlag();
			updateLigthing();
			calledMenu->hide();
			calledMenu->getParent()->show();
			break;
		//default:
		}
	}
	else if(currentMenu == MENU_TITLE_LANGAME) {
		// LAN Spiel
		switch(index) {
		case 0:
			// Erstellen
			gameStatusFlags |=  GAME_STATUS_TYPE_FREE;
			gameStatusFlags &= ~GAME_STATUS_TYPE_DOWN;
			gameStatusFlags &= ~GAME_STATUS_TYPE_SLALOM;
			gameStatusFlags |=  GAME_STATUS_LAN_READY;
			gameStatusFlags |=  GAME_STATUS_LANGAME;
			gameStatusFlags |=  GAME_STATUS_LAN_WAITING;
			gameStatusFlags |=  GAME_STATUS_SERVER;
			setNightFlag();
			playerList[0]->setReadyFlag(true);
			//mainMenu.getCurrentMenu()->setParent(calledMenu);
			//((lobbyMenu*)mainMenu.getCurrentMenu())->setLobbyType(false);
			lanLobby->reset();
			lanLobby->setParent(calledMenu);
			lanLobby->setLobbyType(false);
			calledMenu->hide();
			lanLobby->show();

			// IP-Adressen besorgen und an die Lobby weitergeben
			{
				std::vector<std::string> ipList;	// = new std::vector<std::string>;
				DWORD errorCode;
				IP_ADAPTER_INFO* adapter = NULL;
				ULONG* bufLen = new ULONG;
				*bufLen = 0;
				errorCode = GetAdaptersInfo(adapter, bufLen);	// errorCode == ERROR_BUFFER_OVERFLOW
				_ASSERT(errorCode == ERROR_BUFFER_OVERFLOW);
				adapter = (IP_ADAPTER_INFO*)malloc(*bufLen);
				errorCode = GetAdaptersInfo(adapter, bufLen);
				switch(errorCode) {
				case ERROR_SUCCESS:
					{
						IP_ADAPTER_INFO* currentAdapter = adapter;
						std::clog << "Gefundene Netzwerkadapter:\n";
						while(currentAdapter) {
							IP_ADDR_STRING* ip = &(currentAdapter->IpAddressList);
							bool isActive = false;
							while(ip) {
								std::string buffer = ip->IpAddress.String;
								if(buffer != "0.0.0.0") {
									ipList.push_back(buffer);
									isActive = true;
								}
								ip = ip->Next;
							}
							if(isActive) {
								std::clog << "- - - - - - - - - - -\n";
								std::clog << currentAdapter->Description << ":\n";
#ifdef _DEBUG
								switch(currentAdapter->Type) {
								case MIB_IF_TYPE_ETHERNET:
									std::clog << "Typ: Ethernet\n";
									break;
								case MIB_IF_TYPE_PPP:
									std::clog << "Typ: PPP\n";
									break;
								default:
									std::clog << "Typ: unbekannt, Nr. " << currentAdapter->Type << std::endl;
									break;
							}
#endif
								std::clog << "IP Adressen:\n";
								ip = &(currentAdapter->IpAddressList);
								while(ip) {
									std::clog << '\t' << ip->IpAddress.String << std::endl;
									ip = ip->Next;
								}
							}
							currentAdapter = currentAdapter->Next;
						}
						std::clog << "- - - - - - - - - - -\n";
					}
					break;
#ifdef _DEBUG
				case ERROR_BUFFER_OVERFLOW:
					std::cerr << "Fehler: Buffer Overflow bei GetAdaptersInfo()!\n";
					std::cerr << "\tverlangt: " << *bufLen << std::endl;
					break;
				case ERROR_NOT_SUPPORTED:
					std::cerr << "Fehler: GetAdaptersInfo() nicht unterstuetzt... Mist!\n";
					break;
				case ERROR_NO_DATA:
					std::cerr << "Fehler: No Data bei GetAdaptersInfo()!\n";
					break;
				case ERROR_INVALID_PARAMETER:
					std::cerr << "Fehler: ungueltiger Parameter bei GetAdaptersInfo()!\n";
					break;
				default:
					std::cerr << "Fehler: Unbekannter Fehler >" << errorCode << "< bei GetAdaptersInfo()!\n";
					break;
#endif
				}
				free(adapter);
				delete bufLen;
#ifdef _DEBUG
				std::cout << "\ngefundene Adressen:\n";
				for(unsigned int n = 0; n < ipList.size(); n++)
					std::cout << '\t' << ipList[n] << std::endl;
				std::cout << std::endl;
#endif
				lanLobby->setIpList(&ipList);
				//((lobbyMenu*)mainMenu.getCurrentMenu())->setIpList(ipList);
			}
			break;
		case 1:
			// Teilnehmen
			break;
		case 2:
			// Abbrechen
			gameStatusFlags &= ~GAME_STATUS_LANGAME;
			gameStatusFlags &= ~GAME_STATUS_LAN_WAITING;
			gameStatusFlags &= ~GAME_STATUS_SERVER;
			calledMenu->hide();
			calledMenu->getParent()->show();
			break;
		//default:
		}
	}
	else if(currentMenu == MENU_TITLE_LAN_LOBBY) {
		// LAN: erstellen
		switch(index) {
		case 0:
			// Start
			if(!lanLobby->isClientLobby()) {
				bool allReady = true;
				for(unsigned int i = 0; i < (unsigned int)playerList.size(); i++) {
					if(playerList[i]->getReadyFlag() == false) {
						allReady = false;
						break;
					}
				}
				if(allReady) {
					activateGameMode();
				} else {
					ProtReadyPush *protReadyPush = new ProtReadyPush();
					protReadyPush->giveInformation(0);
					packetProcessor->sendPacket(MAX_PLAYERS, protReadyPush);
				}
			}
			else {
				gameStatusFlags |= GAME_STATUS_LAN_READY;
				playerList[0]->setReadyFlag(true);
				sendInfoReadyFlag(0);
			}
			break;
 		case 1:
			// Beenden
			gameStatusFlags &= ~GAME_STATUS_TYPE_FREE;
			gameStatusFlags &= ~GAME_STATUS_TYPE_DOWN;
			gameStatusFlags &= ~GAME_STATUS_TYPE_SLALOM;
			gameStatusFlags &= ~GAME_STATUS_LANGAME;
			gameStatusFlags &= ~GAME_STATUS_LAN_READY;
			gameStatusFlags &= ~GAME_STATUS_LAN_WAITING;
			gameStatusFlags &= ~GAME_STATUS_LAN_RUNNING;
			gameStatusFlags &= ~GAME_STATUS_SERVER;
			clearPlayerList();
			setNightFlag();
			//updateLigthing();
			calledMenu->hide();
			calledMenu->getParent()->show();
			break;
		case 2:
			// Bot erstellen
			if(!lanLobby->isClientLobby())
				addAIPlayer();
			break;
		case 3:
			// Spieltyp ändern
			if(!lanLobby->isClientLobby()) {
				if(gameStatusFlags & GAME_STATUS_TYPE_FREE) {
					// Freestyle -> Downhill
					gameStatusFlags &= ~GAME_STATUS_TYPE_FREE;
					gameStatusFlags |=  GAME_STATUS_TYPE_DOWN;
					gameStatusFlags &= ~GAME_STATUS_TYPE_SLALOM;
					// send new gameType to all Players
					ProtGameType *protGameType = new ProtGameType();
					protGameType->giveInformation(0, GAME_STATUS_TYPE_DOWN);
					packetProcessor->sendPacket(MAX_PLAYERS, protGameType);
					//lanLobby->setGameType(GAME_STATUS_TYPE_DOWN);
				}
				else {
					// Downhill, * -> Freestyle
					gameStatusFlags |=  GAME_STATUS_TYPE_FREE;
					gameStatusFlags &= ~GAME_STATUS_TYPE_DOWN;
					gameStatusFlags &= ~GAME_STATUS_TYPE_SLALOM;
					// send new gameType to all Players
					ProtGameType *protGameType = new ProtGameType();
					protGameType->giveInformation(0, GAME_STATUS_TYPE_FREE);
					packetProcessor->sendPacket(MAX_PLAYERS, protGameType);
					//lanLobby->setGameType(GAME_STATUS_TYPE_FREE);
				}
			}
			break;
		case 4:
			// Uhrzeit ändern
			if(!lanLobby->isClientLobby()) {
				if(gameStatusFlags & GAME_STATUS_NIGHT)
					gameStatusFlags &= ~GAME_STATUS_NIGHT;
				else
					gameStatusFlags |= GAME_STATUS_NIGHT;
				updateLigthing();
				sendInfoDayTime(MAX_PLAYERS);
			}
			break;
		default:
			// ein Spieler soll gekickt werden
			if(!lanLobby->isClientLobby()) {
				//lobbyMenu* lobby = (lobbyMenu*)calledMenu;
				int idx = lanLobby->getCurrentPlayer();
				if((idx + 1) == playerList.size() && idx != 0)
					mainMenu.changeSelection(-1);
				if(idx >= 0)
					kickPlayer((unsigned int)idx);
			}
		}
	}
	else if(currentMenu == MENU_TITLE_LAN_JOIN) {
		// LAN: teilnehmen
		switch(index) {
		case 0:
			// IP Adresse eingeben
			if(keyboardTarget) {
				if(keyboardInput.length() > 0) {
					if(keyboardInput.length() > MAX_IP_LENGTH)
						keyboardInput.erase(MAX_IP_LENGTH);
					IPAddress = keyboardInput;
				}
				else
					IPAddress = "127.0.0.1";
				{
					std::string buffer = keyboardTargetDefault;
					buffer.append(IPAddress);
					keyboardTarget->setLabel(buffer);
				}
				keyboardTarget = NULL;
				if(keyboardTargetOldValue != keyboardInput)
					configWasChanged = true;
				keyboardTargetDefault.clear();
				keyboardTargetOldValue.clear();
				keyboardInput.clear();
				saveConfig();
			}
			else {
#ifdef _DEBUG
				std::cout << "*** ACHTUNG! ***\nFolgende Buchstaben koennen im Debug-Modus nicht benutzt werden:\n";
				std::cout << "\tI, M, Q, W\n";
#endif
				keyboardTarget = calledMenu->getCurrentEntry();
				keyboardTargetDefault = "IP-Adresse: ";
				keyboardTargetOldValue = IPAddress;
				keyboardInput = keyboardTargetOldValue;
				std::string buffer = std::string(keyboardTargetDefault);
				buffer.append(keyboardTargetOldValue);
				buffer.append(1, MENU_CURSOR_CHAR);
				keyboardTarget->setLabel(buffer);
			}
			break;
		case 1:
			// Verbinden
			gameStatusFlags |=  GAME_STATUS_LAN_WAITING;
			gameStatusFlags |=  GAME_STATUS_LANGAME;
			gameStatusFlags &= ~GAME_STATUS_LAN_READY;
			gameStatusFlags &= ~GAME_STATUS_SERVER;
			gameStatusFlags &= ~GAME_STATUS_TYPE_DOWN;
			gameStatusFlags &= ~GAME_STATUS_TYPE_SLALOM;
			playerList[0]->setReadyFlag(false);
			//mainMenu.getCurrentMenu()->setParent(calledMenu);
			//((lobbyMenu*)mainMenu.getCurrentMenu())->setLobbyType(true);
			lanLobby->reset();
			lanLobby->setParent(calledMenu);
			lanLobby->setLobbyType(true);
			calledMenu->hide();
			lanLobby->show();
            break;
		case 2:
			// Abbrechen
			gameStatusFlags &= ~GAME_STATUS_LAN_WAITING;
			gameStatusFlags &= ~GAME_STATUS_LANGAME;
			calledMenu->hide();
			calledMenu->getParent()->show();
			break;
		//default:
		}
	}
	else if(currentMenu == MENU_TITLE_OPTIONS) {
		// Optionen
		switch(index) {
		case 0:
			// Spieler
			gameStatusFlags |= GAME_STATUS_MENU_PLAYER;
			{
				std::string newLabel = "Kamera: ";
				if(camPos == VIEW_EYE_CAM)
					newLabel.append("EyeCam");
				else if(camPos == VIEW_HELI_CAM)
					newLabel.append("HeliCam");
				else
					newLabel.append("HeadCam");
				mainMenu.getCurrentMenu()->getEntry(7)->setLabel(newLabel);
			}
			break;
		case 1:
			// Video
			{
				stringstream resBuffer, wideBuffer;
				int flags;
				bool isWidescreen;
				getScreenFlags(&flags, &isWidescreen);
				resBuffer  << "Bildschirm: ";
				wideBuffer << "Widescreen: ";
				if(flags != RESOLUTION_CUSTOM) {
					switch(flags) {
					case RESOLUTION_LOW:
						resBuffer << "Niedrig (";
						if(isWidescreen)
							resBuffer << RESOLUTION_LOW_WIDTH_W << "x" << RESOLUTION_LOW_HEIGHT_W << ")";
						else
							resBuffer << RESOLUTION_LOW_WIDTH << "x" << RESOLUTION_LOW_HEIGHT << ")";
						break;
					case RESOLUTION_MED:
						resBuffer << "Mittel (";
						if(isWidescreen)
							resBuffer << RESOLUTION_MED_WIDTH_W << "x" << RESOLUTION_MED_HEIGHT_W << ")";
						else
							resBuffer << RESOLUTION_MED_WIDTH << "x" << RESOLUTION_MED_HEIGHT << ")";
						break;
					default:
						std::cerr	<< "Fehler: unbekanntes Flag >" << flags
									<< "< nach getScreenFlags()!!\n\n";
					case RESOLUTION_HI:
						resBuffer << "Hoch (";
						if(isWidescreen)
							resBuffer << RESOLUTION_HI_WIDTH_W << "x" << RESOLUTION_HI_HEIGHT_W << ")";
						else
							resBuffer << RESOLUTION_HI_WIDTH << "x" << RESOLUTION_HI_HEIGHT << ")";
						break;
					}
					if(isWidescreen)
						wideBuffer << "Ja";
					else
						wideBuffer << "Nein";
				}
				else {
					int width, height;
					getScreenMetrics(&width, &height);
					resBuffer << "Eigene (" << width << "x" << height << ")";
					wideBuffer << "n/a";
				}
				mainMenu.getCurrentMenu()->getEntry(0)->setLabel(resBuffer.str());
				mainMenu.getCurrentMenu()->getEntry(1)->setLabel(wideBuffer.str());
			}
			break;
		case 3:
			// Highscore
			{
				menu* hsListe = mainMenu.getCurrentMenu();
				std::string buffer;
				{
					stringstream temp;
					temp << "Freestyle: " << hsFreestyle.name << " - " << hsFreestyle.value << " SP";
					buffer = temp.str();
				}
				hsListe->getEntry(0)->setLabel(buffer);
				{
					stringstream temp;
					double seconds = hsDownhill.value;
					int min = 0;
					while(seconds >= 60.0) {
						min++;
						seconds -= 60.0;
					}
					temp << "Downhill: " << hsDownhill.name << " - " << min << ":";
					if(seconds < 10.0)
						temp << "0";
					temp.precision(2);
					temp << fixed << seconds;
					buffer = temp.str();
				}
				hsListe->getEntry(1)->setLabel(buffer);
				{
					stringstream temp;
					double seconds = hsSlalom.value;
					int min = 0;
					while(seconds >= 60.0) {
						min++;
						seconds -= 60.0;
					}
					temp << "Slalom: " << hsSlalom.name << " - " << min << ":";
					if(seconds < 10.0)
						temp << "0";
					temp.precision(2);
					temp << fixed << seconds;
					buffer = temp.str();
				}
				hsListe->getEntry(2)->setLabel(buffer);
			}
			break;
		case 4:
			// Fertig
			calledMenu->hide();
			calledMenu->getParent()->show();
			break;
		//default:
		}
	}
	else if(currentMenu == MENU_TITLE_HIGHSCORE) {
		switch(index) {
		case 4:
			calledMenu->hide();
			calledMenu->getParent()->show();
			break;
		}
	}
	else if(currentMenu == MENU_TITLE_CLEAR_HS) {
		switch(index) {
		case 1:
			hsFreestyle.value	= 0.0;
			hsFreestyle.name	= std::string("Niemand");
			hsDownhill.value	= 180.0;
			hsDownhill.name		= std::string("Niemand");
			hsSlalom.value		= 180.0;
			hsSlalom.name		= std::string("Niemand");
			configWasChanged = true;
			saveConfig();
			break;
		default:
			break;
		}
		calledMenu->hide();
		calledMenu->getParent()->show();
		menu* hsListe = mainMenu.getCurrentMenu();
		std::string buffer;
		{
			stringstream temp;
			temp << "Freestyle: " << hsFreestyle.name << " - " << hsFreestyle.value << " SP";
			buffer = temp.str();
		}
		hsListe->getEntry(0)->setLabel(buffer);
		{
			stringstream temp;
			double seconds = hsDownhill.value;
			int min = 0;
			while(seconds >= 60.0) {
				min++;
				seconds -= 60.0;
			}
			temp << "Downhill: " << hsDownhill.name << " - " << min << ":";
			if(seconds < 10.0)
				temp << "0";
			temp.precision(2);
			temp << fixed << seconds;
			buffer = temp.str();
		}
		hsListe->getEntry(1)->setLabel(buffer);
		{
			stringstream temp;
			double seconds = hsSlalom.value;
			int min = 0;
			while(seconds >= 60.0) {
				min++;
				seconds -= 60.0;
			}
			temp << "Slalom: " << hsSlalom.name << " - " << min << ":";
			if(seconds < 10.0)
				temp << "0";
			temp.precision(2);
			temp << fixed << seconds;
			buffer = temp.str();
		}
		hsListe->getEntry(2)->setLabel(buffer);
	}
	else if(currentMenu == MENU_TITLE_OPT_PLAYER) {
		// Optionen: Spieler
		switch(index) {
		case 0:
			playerList[0]->switchPlayerChar();
			switch(playerList[0]->myChar->getType()) {
			case AVATAR_TYPE_BOARDER:
				calledMenu->getCurrentEntry()->setLabel("Spieler: Boarder");
				break;
			case AVATAR_TYPE_CARVER:
				calledMenu->getCurrentEntry()->setLabel("Spieler: Carver");
				break;
			}
			configWasChanged = true;
			break;
		case 1:
			playerList[0]->myChar->randomizeColors(AVATAR_COLOR_HAIR);
			configWasChanged = true;
			break;
		case 2:
			playerList[0]->myChar->randomizeColors(AVATAR_COLOR_SKIN);
			configWasChanged = true;
			break;
		case 3:
			playerList[0]->myChar->randomizeColors(AVATAR_COLOR_SHIRT);
			configWasChanged = true;
			break;
		case 4:
			playerList[0]->myChar->randomizeColors(AVATAR_COLOR_GLOVES);
			configWasChanged = true;
			break;
		case 5:
			playerList[0]->myChar->randomizeColors(AVATAR_COLOR_TROUSERS);
			configWasChanged = true;
			break;
		case 6:
			playerList[0]->myChar->randomizeColors(AVATAR_COLOR_BOOTS);
			configWasChanged = true;
			break;
		case 7:
			if(camPos == VIEW_EYE_CAM) {
				setCamera(CAM_HEAD);
				calledMenu->getCurrentEntry()->setLabel("Kamera: HeadCam");
			}
			else if(camPos == VIEW_HEAD_CAM) {
				setCamera(CAM_HELI);
				calledMenu->getCurrentEntry()->setLabel("Kamera: HeliCam");
			}
			else {
				setCamera(CAM_EYE);
				calledMenu->getCurrentEntry()->setLabel("Kamera: EyeCam");
			}
			configWasChanged = true;
			break;
		case 8:
			if(keyboardTarget) {
				if(keyboardInput.length() > 0) {
					if(keyboardInput.length() > MAX_NAME_LENGTH)
						keyboardInput.erase(MAX_NAME_LENGTH);
				}
				else {
					keyboardInput = "Spieler";
				}
				playerList[0]->setName(keyboardInput);
				std::string buffer = keyboardTargetDefault;
				buffer.append(keyboardInput);
				keyboardTarget->setLabel(buffer);
				keyboardTarget = NULL;
				if(keyboardTargetOldValue != keyboardInput)
					configWasChanged = true;
				keyboardTargetDefault.clear();
				keyboardTargetOldValue.clear();
				keyboardInput.clear();
				saveConfig();
			}
			else {
#ifdef _DEBUG
				std::cout << "*** ACHTUNG! ***\nFolgende Buchstaben koennen im Debug-Modus nicht benutzt werden:\n";
				std::cout << "\tI, M, Q, W\n";
#endif
				keyboardTarget = calledMenu->getCurrentEntry();
				keyboardTargetDefault = "Name: ";
				keyboardTargetOldValue = playerList[0]->getName();
				keyboardInput = keyboardTargetOldValue;
				std::string buffer = std::string(keyboardTargetDefault);
				buffer.append(keyboardTargetOldValue);
				buffer.append(1, MENU_CURSOR_CHAR);
				keyboardTarget->setLabel(buffer);
			}
			break;
		case 9:
			gameStatusFlags &= ~GAME_STATUS_MENU_PLAYER;
			calledMenu->hide();
			calledMenu->getParent()->show();
			saveConfig();
			break;
		//default:
		}
	}
	else if(currentMenu == MENU_TITLE_OPT_VIDEO) {
		// Optionen: Video
		int flags;
		bool isWidescreen;
		getScreenFlags(&flags, &isWidescreen);
		switch(index) {
		case 0:
			{
				int w, h;
				stringstream buffer;
				buffer << "Bildschirm: ";
				switch(flags) {
				case RESOLUTION_LOW:
					if(isWidescreen) {
						w = RESOLUTION_MED_WIDTH_W;
						h = RESOLUTION_MED_HEIGHT_W;
					}
					else {
						w = RESOLUTION_MED_WIDTH;
						h = RESOLUTION_MED_HEIGHT;
					}
					buffer << "Mittel";
					break;
				case RESOLUTION_MED:
					if(isWidescreen) {
						w = RESOLUTION_HI_WIDTH_W;
						h = RESOLUTION_HI_HEIGHT_W;
					}
					else {
						w = RESOLUTION_HI_WIDTH;
						h = RESOLUTION_HI_HEIGHT;
					}
					buffer << "Hoch";
					break;
				default:
				case RESOLUTION_CUSTOM:
				case RESOLUTION_HI:
					if(isWidescreen) {
						w = RESOLUTION_LOW_WIDTH_W;
						h = RESOLUTION_LOW_HEIGHT_W;
					}
					else {
						w = RESOLUTION_LOW_WIDTH;
						h = RESOLUTION_LOW_HEIGHT;
					}
					buffer << "Niedrig";
					break;
				}
				changeResolution(w, h);
				buffer << " (" << w << "x" << h << ")";
				calledMenu->getCurrentEntry()->setLabel(buffer.str());
			}
			configWasChanged = true;
			break;
		case 1:
			if(flags == RESOLUTION_CUSTOM)
				break;	// kann Widescreen nicht umschalten, wenn ich die Auflösung nicht kenne...
			{
				stringstream text;
				text << "Bildschirm: ";
				if(isWidescreen) {
					calledMenu->getCurrentEntry()->setLabel("Widescreen: nein");
					switch(flags) {
					case RESOLUTION_HI:
						changeResolution(RESOLUTION_HI_WIDTH, RESOLUTION_HI_HEIGHT);
						text << "Hoch";
						break;
					case RESOLUTION_MED:
						changeResolution(RESOLUTION_MED_WIDTH, RESOLUTION_MED_HEIGHT);
						text << "Mittel";
						break;
					default:
						std::cerr << "Fehler: falsches RESOLUTION-Flag!\n";
					case RESOLUTION_LOW:
						changeResolution(RESOLUTION_LOW_WIDTH, RESOLUTION_LOW_HEIGHT);
						text << "Niedrig";
						break;
					}
				}
				else {
					calledMenu->getCurrentEntry()->setLabel("Widescreen: ja");
					switch(flags) {
					case RESOLUTION_HI:
						changeResolution(RESOLUTION_HI_WIDTH_W, RESOLUTION_HI_HEIGHT_W);
						text << "Hoch";
						break;
					case RESOLUTION_MED:
						changeResolution(RESOLUTION_MED_WIDTH_W, RESOLUTION_MED_HEIGHT_W);
						text << "Mittel";
						break;
					default:
						std::cerr << "Fehler: falsches RESOLUTION-Flag!\n";
					case RESOLUTION_LOW:
						changeResolution(RESOLUTION_LOW_WIDTH_W, RESOLUTION_LOW_HEIGHT_W);
						text << "Niedrig";
						break;
					}
				}
				int w, h;
				getScreenMetrics(&w, &h);
				text << " (" << w << "x" << h << ")";
				calledMenu->getEntry(0)->setLabel(text.str());
			}
			configWasChanged = true;
			break;
		case 2:
			saveConfig();
			calledMenu->hide();
			calledMenu->getParent()->show();
			break;
		//default:
		}
	}
	else if(currentMenu == MENU_TITLE_CREDITS) {
		// Credits
		calledMenu->hide();
		calledMenu->getParent()->show();
	}
	else if(currentMenu == MENU_TITLE_EXIT) {
		// Beenden
		switch(index) {
		case 0:
			calledMenu->hide();
			calledMenu->getParent()->show();
			break;
		case 1:
			quitGame(0);
			//exit(0);
		//default:
		}
	}
	else {
		// Kein passendes Menü gefunden!!
		std::cout << "\n\n  ***  MountainRanger::menuCallback():  ***\n";
		std::cout << "FEHLER: unbehandelter Fall!\n\n";
		std::cout << "Menu:\t" << currentMenu << "\n";
		std::cout << "Index:\t" << index << "\n\n";
		std::cout << "  ***  ***  ***\n\n";
	}
}

void MountainRanger::createSlalomRoute(unsigned int length) {
	if(gates.size() > 0) {
		for(int i = (int)gates.size() - 1; i >= 0; i--) {
			delete gates[i];
			gates.pop_back();
		}
	}
	SlalomGate* buffer;
	float height;
	Vector3D direction = Vector3D((float)(rand() - (RAND_MAX / 2)), 0.0f, (float)(rand() - (RAND_MAX/2)));
	direction.normalize();
	Vector3D variation, pos, oldPos;
	oldPos = Vector3D(0.0f, 0.0f, 0.0f);
	for(unsigned int i = 0; i < length; i++) {
		variation = Vector3D((float)(rand() - (RAND_MAX / 2)), 0.0f, (float)(rand() - (RAND_MAX/2)));
		variation.normalize();
		variation = variation * SLALOM_GATE_VARIATION;
		//variation += direction * (SLALOM_GATE_DISTANCE * (i+1));

		buffer = new SlalomGate();
		buffer->setColor(Color(0.6f, 0.3f, 0.0f));
		//float factor = SLALOM_GATE_DISTANCE * (i+1);
		pos = Vector3D(	(direction.x * SLALOM_GATE_DISTANCE) + variation.x,
						0.0f,
						(direction.z * SLALOM_GATE_DISTANCE) + variation.z);
		buffer->move(pos);
		height = horn->getHeightOverGround(buffer->getPosition());
		buffer->move(0.0f, -height, 0.0f);

		// Tore passend zum letzten Tor drehen
		Vector3D diff = pos - oldPos;
		diff.y = 0.0f;
		float angle		= diff.getAngle(Vector3D(0.0f, 0.0f, 1.0f));
		float helpAngle	= diff.getAngle(Vector3D(1.0f, 0.0f, 0.0f));
		if(helpAngle > 90.0f)
			angle *= -1.0f;
		buffer->rotate(0.0f, angle, 0.0f);

		oldPos = pos;
		direction += direction;		// "+=" schneller als "* 2.0f"
		gates.push_back(buffer);
#ifdef _DEBUG
		std::clog << "Neues Gate auf ";
		buffer->getPosition().print();
		std::clog << '\n';
#endif
	}
	gates[0]->activate();
	currentGate = 0;

	// Create Display List for current-gate-indicator
	if(glIsList(currentGateIndicatorDL))
		glDeleteLists(currentGateIndicatorDL, 2);
	else {
		currentGateIndicatorDL = glGenLists(2);
		if(currentGateIndicatorDL == 0) {
#ifdef _DEBUG
			std::cerr << "\n\t!!!!!\nKrass, keine DL's mehr frei?!?\n\t!!!!!\n\n";
#endif
			return;
		}
	}

	glNewList(currentGateIndicatorDL, GL_COMPILE);
	{	// Indicator for the current gate
		GLboolean isTexEn = glIsEnabled(GL_TEXTURE_2D);
		glDisable(GL_TEXTURE_2D);
		glPushMatrix();
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		glutSolidCone(0.5, 0.75, (int)GLOBAL_LEVEL_OF_DETAIL, (int)GLOBAL_LEVEL_OF_DETAIL);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		glBegin(GL_TRIANGLE_FAN);
		glNormal3f(0.0f, -1.0f, 0.0f);
		for(int i = 0; i <= GLOBAL_LEVEL_OF_DETAIL + 1.0f; i++) {
			float x, z;
			x = 0.5f * cos(i * (2.0f * PI / GLOBAL_LEVEL_OF_DETAIL));
			z = 0.5f * sin(i * (2.0f * PI / GLOBAL_LEVEL_OF_DETAIL));
			glVertex3f(x, 0.0f, z);
		}
		glEnd();
		glPopMatrix();
		if(isTexEn) glEnable(GL_TEXTURE_2D);
	}
	glEndList();
	glNewList(currentGateIndicatorDL + 1, GL_COMPILE);
	{	// Direction indicator to the next gate
		GLboolean isTexEn = glIsEnabled(GL_TEXTURE_2D);
		glDisable(GL_TEXTURE_2D);
		glPushMatrix();
		glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
		glColor3f(0.8f, 0.8f, 1.0f);
		glutSolidCone(0.25, 0.5, (int)GLOBAL_LEVEL_OF_DETAIL, (int)GLOBAL_LEVEL_OF_DETAIL);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		glBegin(GL_TRIANGLE_FAN);
		glNormal3f(0.0f, 0.0f, 1.0f);
		for(int i = 0; i <= GLOBAL_LEVEL_OF_DETAIL + 1.0f; i++) {
			float x, z;
			x = 0.25f * cos(i * (2.0f * PI / GLOBAL_LEVEL_OF_DETAIL));
			z = 0.25f * sin(i * (2.0f * PI / GLOBAL_LEVEL_OF_DETAIL));
			glVertex3f(x, 0.0f, z);
		}
		glEnd();
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.0f, 0.0f, -0.5f);
		glutSolidCone(0.2, 1.0, (int)GLOBAL_LEVEL_OF_DETAIL, (int)GLOBAL_LEVEL_OF_DETAIL);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		glBegin(GL_TRIANGLE_FAN);
		glNormal3f(0.0f, 0.0f, 1.0f);
		for(int i = 0; i <= GLOBAL_LEVEL_OF_DETAIL + 1.0f; i++) {
			float x, z;
			x = 0.2f * cos(i * (2.0f * PI / GLOBAL_LEVEL_OF_DETAIL));
			z = 0.2f * sin(i * (2.0f * PI / GLOBAL_LEVEL_OF_DETAIL));
			glVertex3f(x, 0.0f, z);
		}
		glEnd();
		glPopMatrix();
		if(isTexEn) glEnable(GL_TEXTURE_2D);
	}
	glEndList();

	for(unsigned int i = 0; i < gates.size(); i++) {
		gates[i]->setIndicator(currentGateIndicatorDL);
	}
}

void MountainRanger::checkGates() {
	if(!(gameStatusFlags & GAME_STATUS_FINISHED)) {
		if(gates[currentGate]->isActive() & SLALOM_GATE_DEACTIVATED)
			currentGate++;
		if(currentGate == gates.size()) {
#ifdef _DEBUG
			std::cout << "keine aktiven Gates.\n";
#endif
			return;
		}
		if(gates[currentGate]->isActive() & SLALOM_GATE_TIMED_OUT) {
			// deactivate gate and move on
			gates[currentGate]->activate(false);
			if(currentGate < gates.size() - 1)
				gates[currentGate + 1]->activate();
			else {
				gameStatusFlags |= GAME_STATUS_FINISHED;
				finalTime[0] = (double)(clock() - startTime[0]) / CLK_TCK;
				playerList[0]->stop(2.0f);
			}
		}

		// Auf Collision überprüfen
		if(*gates[currentGate] && playerList[0]) {
			gates[currentGate]->activate(false);
			if(currentGate < gates.size() - 1)
				gates[currentGate + 1]->activate();
			else {
#ifdef _DEBUG
				std::cout << "Gewonnen, alle Tore durchlaufen!\n";
#endif
				gameStatusFlags |= GAME_STATUS_FINISHED;
				finalTime[0] = (double)(clock() - startTime[0]) / CLK_TCK;
				playerList[0]->stop(2.0f);
			}
		}

		// calculate time with penalty
		if(gameStatusFlags & GAME_STATUS_FINISHED) {
			int missedGates = 0;
			for(unsigned int i = 0; i < gates.size(); i++) {
				if(gates[i]->isActive() & SLALOM_GATE_TIMED_OUT)
					missedGates++;
			}
			//double runTime = (double)(finishTime[0] - startTime[0]) / CLK_TCK;
			double timePenalty = SLALOM_GATE_MISS_PENALTY * missedGates;
			finalTime[0] += timePenalty;
			if(finalTime[0] <= hsSlalom.value) {
				hsSlalom.value = finalTime[0];
				hsSlalom.name = playerList[0]->getName();
				configWasChanged = true;
				saveConfig();
			}
		}
	}
}

#ifdef _DEBUG
void MountainRanger::printQuadsClipped()
{
	horn->printQuadsClipped();
}

void MountainRanger::leakReport()
{
	std::clog << "LeakDetection:\n";
    _CrtDumpMemoryLeaks();
}
#endif
void MountainRanger::processNetworkPacket(char *packet, unsigned int socketNum)
{
	// protocol type
	char prot = packet[0];

	// for unReady-ing
	bool unReady = false;

	// for forwarding
	bool forwardPacket = false;

	// getting playernum
	unsigned int playerNum;
	memcpy(&playerNum, packet+sizeof(char), sizeof(unsigned int));
	playerNum = packetProcessor->serverPlayerNum(playerNum);

	if (playerNum >= playerList.size()) {
#ifdef _DEBUG
		std::clog << "playerNum from Server is too high! ";
		std::clog << "max: " << (unsigned int)playerList.size() - 1;
		std::clog << " // received: " << playerNum;
		std::clog << " // Prot_ID: " << prot << std::endl;
#endif
		return;
	}

#ifdef _DEBUG
	std::clog << "prot: " << prot << std::endl;
#endif

	// analysing packet
	if (prot == PROT_CREATE_PLAYERS_ID) {
		ProtCreatePlayers *currentPacket = new ProtCreatePlayers();
		currentPacket->giveString(packet);
		unsigned int blubb = currentPacket->getCreatePlayers();
		for (unsigned int i = 0; i < currentPacket->getCreatePlayers(); i++) {
			addLanPlayer();
		}
		delete currentPacket;
		unReady = true;
	}

	if (prot == PROT_DELETE_PLAYERS_ID) {
		ProtDeletePlayers *currentPacket = new ProtDeletePlayers();
		currentPacket->giveString(packet);
		for (unsigned int i = 0; i < currentPacket->getDeletePlayers(); i++) {
			if (playerList[(unsigned int)playerList.size() - 1]->getPlayerType() != PLAYER_TYPE_LAN) {
				delete [] packet;
				return;
			}
			delete playerList[playerList.size()-1];
			playerList.pop_back();
			delete style[style.size()-1];
			style.pop_back();
			startTime.pop_back();
			finalTime.pop_back();
#ifdef _DEBUG
			_ASSERT(playerList.size() == style.size());
			_ASSERT(playerList.size() == startTime.size());
			_ASSERT(playerList.size() == finalTime.size());
#endif
		}
		delete currentPacket;
		unReady = true;
	}

	if (prot == PROT_GAME_TYPE_ID) {
		ProtGameType *currentPacket = new ProtGameType();
		currentPacket->giveString(packet);
		if (currentPacket->getGameType() == GAME_STATUS_TYPE_DOWN) {
            // setting to downhill
            gameStatusFlags &= ~GAME_STATUS_TYPE_FREE;
            gameStatusFlags |=  GAME_STATUS_TYPE_DOWN;
            gameStatusFlags &= ~GAME_STATUS_TYPE_SLALOM;
		} else if (currentPacket->getGameType() == GAME_STATUS_TYPE_FREE) {
            // setting to freestyle
			gameStatusFlags |=  GAME_STATUS_TYPE_FREE;
			gameStatusFlags &= ~GAME_STATUS_TYPE_DOWN;
			gameStatusFlags &= ~GAME_STATUS_TYPE_SLALOM;
		}
		delete currentPacket;
		unReady = true;
	}

	if (prot == PROT_SPEED_ANGLE_ID) {
		ProtSpeedAngle *currentPacket = new ProtSpeedAngle();
		currentPacket->giveString(packet);
		playerList[playerNum]->setSpeed(currentPacket->getSpeed());
		playerList[playerNum]->turnPlayer(currentPacket->getAngle() - playerList[playerNum]->getTotalAngle());
		delete currentPacket;
		forwardPacket = true;
	}

	if (prot == PROT_HEADING_ID) {
		ProtHeading *currentPacket = new ProtHeading();
		currentPacket->giveString(packet);
		playerList[playerNum]->setHeading(currentPacket->getHeading());
		playerList[playerNum]->setAirHeading(currentPacket->getAirHeading());
		delete currentPacket;
		forwardPacket = true;
	}

	if (prot == PROT_POSITION_ID) {
		ProtPosition *currentPacket = new ProtPosition();
		currentPacket->giveString(packet);
		playerList[playerNum]->setPosition(currentPacket->getPosition());
		delete currentPacket;
		forwardPacket = true;
	}

	if (prot == PROT_CRASH_ID) {
		ProtCrash *currentPacket = new ProtCrash();
		currentPacket->giveString(packet);
		char crashType = currentPacket->getCrashType();
		unsigned int against = currentPacket->getCrashedAgainst();
		if (crashType == ACTOR_TYPE_PERSON) {
            against = packetProcessor->serverPlayerNum(against);
            playerList[playerNum]->crashAgainst(playerList[against]->getPosition());
            playerList[against]->crashAgainst(playerList[playerNum]->getPosition());
		} else if (crashType == ACTOR_TYPE_OBSTACLE) {
			playerList[playerNum]->crashAgainst(obstacles[against]->getPosition());
		}
        delete currentPacket;
		forwardPacket = true;
	}

	if (prot == PROT_STYLE_POINTS_ID) {
		ProtStylePoints *currentPacket = new ProtStylePoints();
		currentPacket->giveString(packet);
		long extraPoints = currentPacket->getStylePoints() - style[playerNum]->getTotalPoints();
		style[playerNum]->incExtraPoints(extraPoints);
		delete currentPacket;
		forwardPacket = true;
	}

	if (prot == PROT_LAST_FALL_SPEED_ID) {
		ProtLastFallSpeed *currentPacket = new ProtLastFallSpeed();
		currentPacket->giveString(packet);
		float lFallSpeed = currentPacket->getLastFallSpeed();
		playerList[playerNum]->setJumpBonus(lFallSpeed);
		delete currentPacket;
		forwardPacket = true;
	}

	if (prot == PROT_FINAL_TIME_ID) {
		ProtFinalTime *currentPacket = new ProtFinalTime();
		currentPacket->giveString(packet);
		finalTime[playerNum] = currentPacket->getFinalTime();
		delete currentPacket;
		forwardPacket = true;
	}

	if (prot == PROT_START_ID) {
		ProtStart *currentPacket = new ProtStart();
		currentPacket->giveString(packet);
		obstacleSeed = currentPacket->getObsSeed();
		playerStartAngle = currentPacket->getPlayerStartAngle();
		activateGameMode();
		delete currentPacket;
	}

	if (prot == PROT_DAY_TIME_ID) {
 		ProtDayTime *currentPacket = new ProtDayTime();
		currentPacket->giveString(packet);
		if (currentPacket->getNight() == GAME_STATUS_NIGHT) {
			gameStatusFlags |= GAME_STATUS_NIGHT;
		} else {
			gameStatusFlags &= ~GAME_STATUS_NIGHT;
		}
		updateLigthing();
		delete currentPacket;
		unReady = true;
	}

	if (prot == PROT_READY_ID) {
		ProtReady *currentPacket = new ProtReady();
		currentPacket->giveString(packet);
		playerList[playerNum]->setReadyFlag(currentPacket->getReady());
		delete currentPacket;
		forwardPacket = true;
	}

	if (prot == PROT_READY_PUSH_ID) {
		// set message in lobby
		if (!playerList[0]->getReadyFlag()) lanLobby->displayMessage("Bitte auf Bereit wechseln!");
	}

	if (prot == PROT_VERSION_ID) {
		ProtVersion *currentPacket = new ProtVersion();
		currentPacket->giveString(packet);
		if (currentPacket->getVersion() < MINIMUM_VERSION) {
			// kick player
			ProtError *protError = new ProtError();
			protError->giveInformation(0, SERVER__CLIENT_WRONG_VERSION);
			packetProcessor->sendPacket(playerNum, protError);
			packetProcessor->addSocketToLateDelete(playerNum);
		}
		delete currentPacket;
	}

	if (prot == PROT_ERROR_ID) {
		ProtError *currentPacket = new ProtError();
		currentPacket->giveString(packet);
		networkError(currentPacket->getError());
		delete currentPacket;
	}

	if (prot == PROT_PING_ID) {
		packetProcessor->pingRecv(playerNum);
	}

	if (prot == PROT_LOBBY_ID) {
		activateMainMenu();
	}

	//if (prot == PROT_PLAYER_STOP_ID) {
	//	//ProtFinish *currentPacket = new ProtFinish();
	//	//currentPacket->giveString(packet);
	//	//TODO: finish game
	//}

	if (prot == PROT_INIT_INFORMATION_ID) {
		ProtInitInformation *currentPacket = new ProtInitInformation();
		currentPacket->giveString(packet);
		// Name
		playerList[playerNum]->setName(currentPacket->getName());
		// Avatar Type
		if (playerList[playerNum]->myChar->getType() != currentPacket->getAvatarType()) playerList[playerNum]->switchPlayerChar();
		// Color
		playerList[playerNum]->myChar->setColor(currentPacket->getHair(), AVATAR_COLOR_HAIR);
		playerList[playerNum]->myChar->setColor(currentPacket->getSkin(), AVATAR_COLOR_SKIN);
		playerList[playerNum]->myChar->setColor(currentPacket->getShirt(), AVATAR_COLOR_SHIRT);
		playerList[playerNum]->myChar->setColor(currentPacket->getGloves(), AVATAR_COLOR_GLOVES);
		playerList[playerNum]->myChar->setColor(currentPacket->getTrousers(), AVATAR_COLOR_TROUSERS);
		playerList[playerNum]->myChar->setColor(currentPacket->getBoots(), AVATAR_COLOR_BOOTS);
		delete currentPacket;
		forwardPacket = true;
	}

	if (prot == PROT_PLAYER_NUM_ID) {
		if (gameStatusFlags & GAME_STATUS_SERVER) {
			delete [] packet;
			return;
		}
		packetProcessor->changePlayerNum(packetProcessor->serverPlayerNum(0), playerNum);
		packetProcessor->setWhoAmI(playerNum);
		// when we receive a playerNum then we send the init
		sendInit(0);
		// send the game version
		ProtVersion *protVersion = new ProtVersion();
		protVersion->giveInformation(packetProcessor->serverPlayerNum(0), CURRENT_VERSION);
		packetProcessor->sendPacket(0, protVersion);
#ifdef _DEBUG_NETWORK
		std::cout << "Playernum from Server received: " << playerNum << std::endl;
#endif
	}

	if ((gameStatusFlags & GAME_STATUS_SERVER) && (forwardPacket))
		networkUpdate(packet, playerNum);

	delete [] packet;

	if (unReady) {
		// in client mode: tell server: i am not ready anymore
		if (!(gameStatusFlags & GAME_STATUS_SERVER) && (packetProcessor->serverPlayerNum(0) != 0)) {
			playerList[0]->setReadyFlag(false);
			sendInfoReadyFlag(0);
		}
	}
}

void MountainRanger::networkUpdate(char* packet, unsigned int playerNum)
{
	for (unsigned int i = 0; i < playerList.size(); i++) {
		if ((playerList[i]->getPlayerType() == PLAYER_TYPE_LAN) && (i != playerNum)) {
			ProtCopy *protCopy = new ProtCopy(packet);
			packetProcessor->sendPacket(i, protCopy);
		}
	}
}

void MountainRanger::networkError(unsigned int errorID) {
	if (gameStatusFlags & GAME_STATUS_SERVER) return;

	// Netzwerkmodus verlassen
	gameStatusFlags &= ~GAME_STATUS_LANGAME;
	gameStatusFlags &= ~GAME_STATUS_LAN_WAITING;
	// Lobby verlassen
	menu* msgTarget = lanLobby->getParent();
	setNightFlag();
	lanLobby->hide();
	msgTarget->show();

	switch(errorID) {
	case CLIENT__SERVER_NOT_FOUND:
		// Meldung ausgeben
		msgTarget->displayMessage("Fehler: Server nicht gefunden!");
		break;
	case SERVER__CLIENT_WRONG_VERSION:
		// Meldung ausgeben
		{
			std::stringstream buffer;
			buffer << "Fehler: Falsche Version! (V1." << CURRENT_VERSION << ')';
			msgTarget->displayMessage(buffer.str());
		}
		break;
	case SERVER__CLIENT_KICKED:
		// Meldung ausgeben
		msgTarget->displayMessage("Sie wurden ausgeschlossen!");
		break;
	case SERVER__SERVER_FULL:
		// Meldung ausgeben
		msgTarget->displayMessage("Server ist voll!");
		break;
	case SERVER_CLIENT__TIME_OUT:
        // Meldung ausgeben
		msgTarget->displayMessage("Verbindung verloren (Time-Out)!");
		break;
	default:
#ifdef _DEBUG
		std::cerr << "\t*** Fehler! ***\nunbekannter Netzwerkfehler: " << errorID << "\n\n";
#endif
		// Meldung ausgeben
		msgTarget->displayMessage("unbekannter Fehler!");
		break;
	}
}

bool MountainRanger::networkAllPlayersReady() const {
	bool allSet = true;
	for(unsigned int i = 0; i < playerList.size(); i++) {
		if(playerList[i]->getPlayerType() != PLAYER_TYPE_AI
			&& !playerList[i]->getReadyFlag()) {
				allSet = false;
				break;
		}
	}
	return allSet;
}

void MountainRanger::sendSelfAndAI(float timeDiff)
{
	// no connection open -> leave
	if (!packetProcessor->connectionsOpen()) return;

	// Ping also in lobby
	lastNetworkPing += timeDiff;
	if (lastNetworkPing >= NETWORK_PING_SEND_TIME) {
		lastNetworkPing = 0.0f;
		ProtPing *protPing = new ProtPing();
		protPing->giveInformation(packetProcessor->serverPlayerNum(0));
		packetProcessor->sendPacket(MAX_PLAYERS, protPing);
	}

	// if lobby -> leave now
	if (gameStatusFlags & GAME_STATUS_LAN_WAITING) return;

	// sending-decisions
	bool sendSpeedAngle = false;
	bool sendHeading = false;
	bool sendPosition = false;
	bool sendStylePoints = false;

	lastNetworkUpdateSpeedAngle += timeDiff;
	if (lastNetworkUpdateSpeedAngle >= NETWORK_UPDATE_SPEED_ANGLE) {
		sendSpeedAngle = true;
		lastNetworkUpdateSpeedAngle = 0.0f;
	}
	lastNetworkUpdateHeading += timeDiff;
	if (lastNetworkUpdateHeading >= NETWORK_UPDATE_HEADING) {
		sendHeading = true;
		lastNetworkUpdateHeading = 0.0f;
	}
	lastNetworkUpdatePosition += timeDiff;
	if (lastNetworkUpdatePosition >= NETWORK_UPDATE_POSITION) {
		sendPosition = true;
		lastNetworkUpdatePosition = 0.0f;
	}
	lastNetworkUpdateStylePoints += timeDiff;
	if (lastNetworkUpdateStylePoints >= NETWORK_UPDATE_STYLE_POINTS) {
		sendStylePoints = true;
		lastNetworkUpdateStylePoints = 0.0f;
	}

	if (!(gameStatusFlags & GAME_STATUS_FINISHED)) sendFinalTime = true;

	// server-mode
	if (gameStatusFlags & GAME_STATUS_SERVER) {
        for (unsigned int i = 0; i < playerList.size(); i++) {
            if (playerList[i]->getPlayerType() == PLAYER_TYPE_LAN) continue;
			if (sendSpeedAngle)		sendInfoSpeedAngle(i);
			if (sendHeading)		sendInfoHeading(i);
			if (sendPosition)		sendInfoPosition(i);
			if (sendStylePoints)	sendInfoStylePoints(i);
			if ((gameStatusFlags & GAME_STATUS_FINISHED) && (finalTime[0] != 0.0)) {
				sendInfoFinalTime(i);
				sendFinalTime = false;
			}
		}
	}

	// client-mode
	if (!(gameStatusFlags & GAME_STATUS_SERVER)) {
		if (sendSpeedAngle)		sendInfoSpeedAngle(0);
		if (sendHeading)		sendInfoHeading(0);
		if (sendPosition)		sendInfoPosition(0);
		if (sendStylePoints)	sendInfoStylePoints(0);
		if ((gameStatusFlags & GAME_STATUS_FINISHED) && (finalTime[0] != 0.0)) {
			sendInfoFinalTime(0);
			sendFinalTime = false;
		}
	}
}

void MountainRanger::sendInfoSpeedAngle(unsigned int playerNum)
{
	// checking if player already stopped
	if (playerList[playerNum]->isStopped()) return;

	// sending speed/angle
	float speed = playerList[playerNum]->getSpeed();
	float angle = playerList[playerNum]->getTotalAngle();
	ProtSpeedAngle *newPacketSpeedAngle = new ProtSpeedAngle();
	newPacketSpeedAngle->giveInformation(packetProcessor->serverPlayerNum(playerNum), speed, angle);
	packetProcessor->sendPacket(MAX_PLAYERS, newPacketSpeedAngle);
}

void MountainRanger::sendInfoHeading(unsigned int playerNum)
{
	// checking if player already stopped
	if (playerList[playerNum]->isStopped()) return;

	// sending heading
	ProtHeading *newPacketHeading = new ProtHeading();
	Vector3D heading = playerList[playerNum]->getHeading();
	Vector3D airHeading = playerList[playerNum]->getAirHeading();
	newPacketHeading->giveInformation(packetProcessor->serverPlayerNum(playerNum), heading, airHeading);
	packetProcessor->sendPacket(MAX_PLAYERS, newPacketHeading);
}

void MountainRanger::sendInfoPosition(unsigned int playerNum)
{
	// checking if player already stopped
	if (playerList[playerNum]->isStopped()) return;

	// position
	Vector3D position = playerList[playerNum]->getPosition();
	ProtPosition *newPacketPosition = new ProtPosition();
	newPacketPosition->giveInformation(packetProcessor->serverPlayerNum(playerNum), position);
	packetProcessor->sendPacket(MAX_PLAYERS, newPacketPosition);
}

void MountainRanger::sendInfoStylePoints(unsigned int playerNum)
{
	// stylePoints
	long stylePoints = style[playerNum]->getTotalPoints();
	ProtStylePoints *protStylePoints = new ProtStylePoints();
	protStylePoints->giveInformation(packetProcessor->serverPlayerNum(playerNum), stylePoints);
	packetProcessor->sendPacket(MAX_PLAYERS, protStylePoints);
}

void MountainRanger::sendInfoFinalTime(unsigned int playerNum)
{
	// finalTime
	ProtFinalTime *protFinalTime = new ProtFinalTime();
	protFinalTime->giveInformation(packetProcessor->serverPlayerNum(playerNum), finalTime[playerNum]);
	packetProcessor->sendPacket(MAX_PLAYERS, protFinalTime);

}

void MountainRanger::sendInfoDayTime(unsigned int playerNum)
{
    // tell player the daytime
    ProtDayTime *protDayTime = new ProtDayTime();
    protDayTime->giveInformation(0, gameStatusFlags & GAME_STATUS_NIGHT);
    packetProcessor->sendPacket(playerNum, protDayTime);
}

void MountainRanger::sendInfoReadyFlag(unsigned int playerNum)
{
	ProtReady *protReady = new ProtReady();
	protReady->giveInformation(packetProcessor->serverPlayerNum(playerNum), playerList[playerNum]->getReadyFlag());
	packetProcessor->sendPacket(MAX_PLAYERS, protReady);
}

bool MountainRanger::packetsReady(unsigned int socketNum)
{
	return packetProcessor->packetsReady(socketNum);
}

char* MountainRanger::getPacket(unsigned int socketNum)
{
	return packetProcessor->getPacket(socketNum);
}

bool MountainRanger::socketToClose()
{
	return packetProcessor->socketToDelete();
}

unsigned int MountainRanger::getSocketToClose()
{
	return packetProcessor->getSocketToDelete();
}

bool MountainRanger::createPlayer()
{
	// if the game is in server mode
	if ((gameStatusFlags & GAME_STATUS_LAN_WAITING) && (gameStatusFlags & GAME_STATUS_SERVER)) {
		if (playerList.size() >= MAX_PLAYERS) {
			// tell player "server is full"
			unsigned int sum = 1;
			while (packetProcessor->playerExists(MAX_PLAYERS+sum))
				sum++;
			packetProcessor->addPlayerSocket(MAX_PLAYERS+sum);
			ProtError *protError = new ProtError();
			protError->giveInformation(0, SERVER__SERVER_FULL);
			packetProcessor->sendPacket(MAX_PLAYERS+sum, protError);
			packetProcessor->addSocketToLateDelete(MAX_PLAYERS+sum);
#ifdef _DEBUG
			std::clog << "Server full! sent to client\n";
#endif
			return true;
		}

#ifdef _DEBUG
		std::clog << "createPlayer() in Servermode, sending playerNum ";
#endif
		// create player and tell packetProcessor
		unsigned int newPlayerNumber = addLanPlayer();
		packetProcessor->addPlayerSocket(newPlayerNumber);

		// let Player create enough Players on playerList
		ProtCreatePlayers *protCreatePlayers = new ProtCreatePlayers();
		protCreatePlayers->giveInformation(0, (unsigned int)playerList.size() - 1);
		packetProcessor->sendPacket(newPlayerNumber, protCreatePlayers);

		// tell player which number he has
		ProtPlayerNum *playerProt = new ProtPlayerNum();
		playerProt->giveInformation(newPlayerNumber);
		packetProcessor->sendPacket(newPlayerNumber, playerProt);
#ifdef _DEBUG
		std::clog << newPlayerNumber << std::endl;
#endif

		// tell player the gameType
		ProtGameType *protGameType = new ProtGameType();
		if(gameStatusFlags & GAME_STATUS_TYPE_FREE) {
			protGameType->giveInformation(0, GAME_STATUS_TYPE_FREE);
		} else if (gameStatusFlags & GAME_STATUS_TYPE_DOWN) {
			protGameType->giveInformation(0, GAME_STATUS_TYPE_DOWN);
		} else return false;
		packetProcessor->sendPacket(newPlayerNumber, protGameType);

		// send current dayTime
		sendInfoDayTime(newPlayerNumber);

		// send player all other players
		for (unsigned int k = 0; k < newPlayerNumber; k++) {
			// Init Information
			ProtInitInformation *packet = new ProtInitInformation();
			// TODO: eleganter lösen, auf std::string oder const char* umstellen
				char* buffer = new char[playerList[k]->getName().length() + 1];
				playerList[k]->getName().copy(buffer, playerList[k]->getName().length());
				buffer[playerList[k]->getName().length()] = '\0';
			packet->giveInformation(k, buffer, playerList[k]->myChar->getType(), playerList[k]->myChar->getColor(AVATAR_COLOR_HAIR), playerList[k]->myChar->getColor(AVATAR_COLOR_SKIN), playerList[k]->myChar->getColor(AVATAR_COLOR_SHIRT), playerList[k]->myChar->getColor(AVATAR_COLOR_GLOVES), playerList[k]->myChar->getColor(AVATAR_COLOR_TROUSERS), playerList[k]->myChar->getColor(AVATAR_COLOR_BOOTS));
			packetProcessor->sendPacket(newPlayerNumber, packet);
			// readyFlag
			ProtReady *packetReady = new ProtReady();
			packetReady->giveInformation(k, playerList[k]->getReadyFlag());
			packetProcessor->sendPacket(newPlayerNumber, packetReady);
			//send each player a create player signal
			ProtCreatePlayers *protCreatePlayers = new ProtCreatePlayers();
			protCreatePlayers->giveInformation(0, 1);
			packetProcessor->sendPacket(k, protCreatePlayers);
		}

		// new Player will return to playerNum the initInformation, and it will be sent to
		// all other players, so we dont do it here!
		return true;
	}

	// client mode
	if ((gameStatusFlags & GAME_STATUS_LAN_WAITING) && !(gameStatusFlags & GAME_STATUS_SERVER)) {
		// add socket, but waiting for playerNum
		if (packetProcessor->playerExists(0)) return false;
		packetProcessor->addPlayerSocket(0);
#ifdef _DEBUG
		std::clog << "createPlayer() in Client-Mode, not sending playerNum\n";
#endif
		return true;
	}

	if (!(gameStatusFlags & GAME_STATUS_LAN_WAITING)) {
#ifdef _DEBUG_NETWORK
		std::clog << "client wants to connect, but server is closed\n";
#endif
		return false;
	}

	return false;
}

void MountainRanger::killPlayer(unsigned int socketNum)
{
	// first of all tell packetProcessor to kill the socket from list
	unsigned int playerNum = packetProcessor->deleteSocket(socketNum);

	// in server mode
	if (gameStatusFlags & GAME_STATUS_SERVER) {
		if (playerNum >= MAX_PLAYERS) return;
		if (gameStatusFlags & GAME_STATUS_LAN_WAITING) {
			delete playerList[playerNum];
			delete style[playerNum];
			std::vector<Player*>::iterator		itP  = playerList.begin();
			std::vector<stylePoints*>::iterator	itS  = style.begin();
			std::vector<clock_t>::iterator		itSt = startTime.begin();
			std::vector<double>::iterator		itFt = finalTime.begin();
			for (unsigned int i = 0; i < playerNum; i++) {
				itP++;
				itS++;
				itSt++;
				itFt++;
			}
			playerList.erase(itP);
			style.erase(itS);
			startTime.erase(itSt);
			finalTime.erase(itFt);
			updateAllPlayers(playerNum);
		}
		else {
			// change player in playerList for KI
			Player *toBeDeleted = playerList[playerNum];
			//set AImode
			AIPlayer *newAIPlayer = new AIPlayer(toBeDeleted);
			if (gameStatusFlags & GAME_STATUS_TYPE_DOWN)
				newAIPlayer->setGameMode(AI_GAME_TYPE_DOWN);
			else
				newAIPlayer->setGameMode(AI_GAME_TYPE_FREE);
			// set player to aiplayer and delete lanplayer
			playerList[playerNum] = newAIPlayer;
			if (toBeDeleted->isStopped()) playerList[playerNum]->stop();
			delete toBeDeleted;

			std::string newName;
			newName.append("(");
			newName.append(playerList[playerNum]->getName());
			if(newName.size() >= MAX_NAME_LENGTH)
				newName.resize(MAX_NAME_LENGTH - 1);
			newName.append(")");

			playerList[playerNum]->setName(newName);
			playerList[playerNum]->setReadyFlag(true);
			sendInit(playerNum);
			sendInfoReadyFlag(playerNum);
		}
	}
	else {
		// if there was a timeout, pass it to networkError
		if (packetProcessor->gotTimeOut()) networkError(SERVER_CLIENT__TIME_OUT);

		bool lostConnection = false;
		if(gameStatusFlags & GAME_STATUS_LANGAME)
			lostConnection = true;
		if(gameIsRunning())
			activateMainMenu();	// exit game
		// kill other players
		clearPlayerList();

		// delete flags
		gameStatusFlags  = GAME_STATUS_NONE;
		gameStatusFlags |= GAME_STATUS_MENU;

		// leave lobby
		setNightFlag();
		lanLobby->hide();
		lanLobby->getParent()->show();
		if(lostConnection)
			lanLobby->getParent()->displayMessage("Verbindung verloren!");
		packetProcessor->setWhoAmI(0);
	}
}

bool MountainRanger::kickPlayer(unsigned int playerNum)
{
	if (playerNum == 0) return false;
	if (playerNum >= playerList.size()) return false;

	// if type == lanPlayer, just set socketToKill, rest will be done in killPlayer
	// after networkIO closes socket
	if (playerList[playerNum]->getPlayerType() == PLAYER_TYPE_LAN) {
		// tell client that he was kicked
		ProtError *protError = new ProtError();
		protError->giveInformation(0, SERVER__CLIENT_KICKED);
		packetProcessor->sendPacket(playerNum, protError);
		// let socket be deleted
		packetProcessor->addSocketToDelete(playerNum);
		return true;
	}
	else if (playerList[playerNum]->getPlayerType() == PLAYER_TYPE_AI) {
		// Player
		{
			delete playerList[playerNum];
			std::vector<Player*>::iterator it = playerList.begin();
			for (unsigned int i = 0; i < playerNum; i++)
				it++;
			playerList.erase(it);
		}
		// Style Points
		{
			delete style[playerNum];
			std::vector<stylePoints*>::iterator it = style.begin();
			for (unsigned int i = 0; i < playerNum; i++)
				it++;
			style.erase(it);
		}
		// start time
		{
			std::vector<clock_t>::iterator it = startTime.begin();
			for (unsigned int i = 0; i < playerNum; i++)
				it++;
			startTime.erase(it);
		}
		// final time
		{
			std::vector<double>::iterator it = finalTime.begin();
			for (unsigned int i = 0; i < playerNum; i++)
				it++;
			finalTime.erase(it);
		}

		updateAllPlayers(playerNum);
		return true;
	}

	return false;
}

void MountainRanger::clearPlayerList() {
	for(unsigned int i = 1; i < playerList.size(); i++)
		delete playerList[i];
	Player* human = playerList[0];	// save human player...
	playerList.clear();
	playerList.push_back(human);	// ...and restore him.
	human->setReadyFlag(false);
	startTime.clear();
	finalTime.clear();
	int size = (int)style.size();
	for(int i = 0; i < size; i++)
		delete style[i];
	style.clear();
	// init values for human player
	startTime.push_back(0);
	finalTime.push_back(0);
	style.push_back(new stylePoints());
}

void MountainRanger::updateAllPlayers(unsigned int start)
{
	for (unsigned int i = start; i < playerList.size(); i++) {
		// if player exists: it is a lan player
		if (packetProcessor->playerExists(i+1)) {
			// change in packetProcessor the old playerNum to new
			packetProcessor->changePlayerNum(i+1, i);
			// tell player which new number he has
			ProtPlayerNum *playerProt = new ProtPlayerNum();
			playerProt->giveInformation(i);
			packetProcessor->sendPacket(i, playerProt);
		} else {
			sendInit(i);
		}
		sendInfoReadyFlag(i);
	}
	// tell player to drop last player from playerList
	ProtDeletePlayers *protDeletePlayers = new ProtDeletePlayers();
	protDeletePlayers->giveInformation(0, 1);
	packetProcessor->sendPacket(MAX_PLAYERS, protDeletePlayers);
	// send self, cause on all lanMachines self maybe after killedPlayer
	sendInit(0);
}

void MountainRanger::sendInit(unsigned int playerNum)
{
	ProtInitInformation *currentPacket = new ProtInitInformation();
	// TODO: eleganter lösen, auf std::string oder const char* umstellen
		char* buffer = new char[playerList[playerNum]->getName().length() + 1];
		playerList[playerNum]->getName().copy(buffer, playerList[playerNum]->getName().length());
		buffer[playerList[playerNum]->getName().length()] = '\0';
		currentPacket->giveInformation(packetProcessor->serverPlayerNum(playerNum), buffer, playerList[playerNum]->myChar->getType(), playerList[playerNum]->myChar->getColor(AVATAR_COLOR_HAIR), playerList[playerNum]->myChar->getColor(AVATAR_COLOR_SKIN), playerList[playerNum]->myChar->getColor(AVATAR_COLOR_SHIRT), playerList[playerNum]->myChar->getColor(AVATAR_COLOR_GLOVES), playerList[playerNum]->myChar->getColor(AVATAR_COLOR_TROUSERS), playerList[playerNum]->myChar->getColor(AVATAR_COLOR_BOOTS));
		delete [] buffer;
		buffer = NULL;
	packetProcessor->sendPacket(MAX_PLAYERS, currentPacket);
}

std::string MountainRanger::getIPAddress()
{
	return IPAddress;
}

bool MountainRanger::runClient()
{
	// if (langame && !server) return true
	if ((gameStatusFlags & GAME_STATUS_LANGAME) && (!(gameStatusFlags & GAME_STATUS_SERVER)))
		return true;
	return false;
}

bool MountainRanger::runServer()
{
	if ((gameStatusFlags & GAME_STATUS_LANGAME) && (gameStatusFlags & GAME_STATUS_SERVER))
		return true;
	return false;
}

/**
 * loads the user configuration from the USER_CONFIG_FILE and
 * writes the data. If no USER_CONFIG_FILE is found, default
 * values are set.
 */
void MountainRanger::loadConfig() {
	fstream userConfig;
	char buffer[MAX_PATH];
	SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, buffer);
	std::string fullPath = buffer;
	fullPath.append("\\");
	fullPath.append(USER_CONFIG_FILE);
	// fullPath should now be something like
	// "C:\Documents and Settings\JoeUser\My Documents\wintergames.cfg"

	userConfig.open(fullPath.c_str(), ios::in);
	if(userConfig.good()) {
		char* buffer = new char[FILE_LINE_MAX];
		unsigned int c;
		int res[] = {0, 0};
		while(!userConfig.eof()) {
			c = 0;
			while(!userConfig.eof() && userConfig.peek() != (int)CONFIG_DELIMITER && userConfig.peek() != '\n' && c < FILE_LINE_MAX - 1) {
				buffer[c] = userConfig.get();
				c++;
			}
			buffer[c] = '\0';
			if(buffer[0] != FILE_COMMENT_CHAR && !userConfig.eof() && userConfig.peek() != '\n') {
				userConfig.ignore();	// CONFIG_DELIMITER
				if(strcmp(buffer, CONFIG_PREFIX_TYPE) == 0) {
					userConfig.getline(buffer, FILE_LINE_MAX);
					int type = atoi(buffer);
					if(type != playerList[0]->myChar->getType())
						playerList[0]->switchPlayerChar();
				}
				else if(strcmp(buffer, CONFIG_PREFIX_NAME) == 0) {
					userConfig.getline(buffer, FILE_LINE_MAX);
					//char* temp = new char[strlen(buffer) + 1];
					//memcpy(temp, buffer, strlen(buffer) + 1);
					playerList[0]->setName(buffer);
				}
				else if(strcmp(buffer, CONFIG_PREFIX_COL_HAIR) == 0) {
					float r, g, b;
					userConfig >> r >> g >> b;
					userConfig.ignore(FILE_LINE_MAX, '\n');
					playerList[0]->myChar->setColor(Color(r, g, b), AVATAR_COLOR_HAIR);
				}
				else if(strcmp(buffer, CONFIG_PREFIX_COL_SKIN) == 0) {
					float r, g, b;
					userConfig >> r >> g >> b;
					userConfig.ignore(FILE_LINE_MAX, '\n');
					playerList[0]->myChar->setColor(Color(r, g, b), AVATAR_COLOR_SKIN);
				}
				else if(strcmp(buffer, CONFIG_PREFIX_COL_SHIRT) == 0) {
					float r, g, b;
					userConfig >> r >> g >> b;
					userConfig.ignore(FILE_LINE_MAX, '\n');
					playerList[0]->myChar->setColor(Color(r, g, b), AVATAR_COLOR_SHIRT);
				}
				else if(strcmp(buffer, CONFIG_PREFIX_COL_GLOVES) == 0) {
					float r, g, b;
					userConfig >> r >> g >> b;
					userConfig.ignore(FILE_LINE_MAX, '\n');
					playerList[0]->myChar->setColor(Color(r, g, b), AVATAR_COLOR_GLOVES);
				}
				else if(strcmp(buffer, CONFIG_PREFIX_COL_TROUSERS) == 0) {
					float r, g, b;
					userConfig >> r >> g >> b;
					userConfig.ignore(FILE_LINE_MAX, '\n');
					playerList[0]->myChar->setColor(Color(r, g, b), AVATAR_COLOR_TROUSERS);
				}
				else if(strcmp(buffer, CONFIG_PREFIX_COL_BOOTS) == 0) {
					float r, g, b;
					userConfig >> r >> g >> b;
					userConfig.ignore(FILE_LINE_MAX, '\n');
					playerList[0]->myChar->setColor(Color(r, g, b), AVATAR_COLOR_BOOTS);
				}
				else if(strcmp(buffer, CONFIG_PREFIX_LAST_IP) == 0) {
					userConfig.getline(buffer, FILE_LINE_MAX);
					IPAddress = std::string(buffer);
				}
				else if(strcmp(buffer, CONFIG_PREFIX_HS_FREE) == 0) {
					long value;
					userConfig >> value >> hsFreestyle.name;
					hsFreestyle.name = cryptHighscore(hsFreestyle.name);
					hsFreestyle.value = (double)cryptHighscore(value);
					userConfig.ignore(FILE_LINE_MAX, '\n');
				}
				else if(strcmp(buffer, CONFIG_PREFIX_HS_DOWN) == 0) {
					long value;
					userConfig >> value >> hsDownhill.name;
					hsDownhill.name = cryptHighscore(hsDownhill.name);
					hsDownhill.value = ((double)cryptHighscore(value)) / 1000.0;
					userConfig.ignore(FILE_LINE_MAX, '\n');
				}
				else if(strcmp(buffer, CONFIG_PREFIX_HS_SLALOM) == 0) {
					long value;
					userConfig >> value >> hsSlalom.name;
					hsSlalom.name = cryptHighscore(hsSlalom.name);
					hsSlalom.value = ((double)cryptHighscore(value)) / 1000.0;
					userConfig.ignore(FILE_LINE_MAX, '\n');
				}
				else if(strcmp(buffer, CONFIG_PREFIX_CAM) == 0) {
					int newAngle;
					userConfig >> newAngle;
					setCamera(newAngle);
					userConfig.ignore(FILE_LINE_MAX, '\n');
				}
				else if(strcmp(buffer, CONFIG_PREFIX_RES_WIDTH) == 0) {
					userConfig >> res[0];
					userConfig.ignore(FILE_LINE_MAX, '\n');
				}
				else if(strcmp(buffer, CONFIG_PREFIX_RES_HEIGHT) == 0) {
					userConfig >> res[1];
					userConfig.ignore(FILE_LINE_MAX, '\n');
				}
			}
			else {
				// Read ans ignore rest of line
				userConfig.ignore(FILE_LINE_MAX, '\n');
				//userConfig.getline(buffer, FILE_LINE_MAX);
			}
		}
		delete [] buffer;
		if(res[0] != 0 && res[1] != 0)
			changeResolution(res[0], res[1]);
	}
	else {
		std::cerr << "Fehler: kann die Datei \"" << fullPath << "\" nicht laden!\n";
		// loading defaults
		playerList[0]->setName("Spieler");
		// colors and type are set to default by Avatar-Constructor
		IPAddress = "127.0.0.1";
		setCamera(CAM_HEAD);
		hsFreestyle.value	= 0.0;
		hsFreestyle.name	= std::string("Niemand");
		hsDownhill.value	= 180.0;
		hsDownhill.name		= std::string("Niemand");
		hsSlalom.value		= 180.0;
		hsSlalom.name		= std::string("Niemand");
	}
	userConfig.close();
}

bool MountainRanger::saveConfig() {
	fstream userConfig;
	bool successful = false;
	char buffer[MAX_PATH];
	SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, buffer);
	std::string fullPath = buffer;
	fullPath.append("\\");
	fullPath.append(USER_CONFIG_FILE);
	// fullPath should now be something like
	// "C:\Documents and Settings\JoeUser\My Documents\wintergames.cfg"

#ifdef _DEBUG
	std::clog << "mRanger::saveConfig():\n";
	std::clog << "\tDatei: " << fullPath << "\n";
	std::clog << "\tStatus: ";
#endif

	if(!configWasChanged) {
		// Nothing to do...
#ifdef _DEBUG
		std::clog << "nichts zu tun...";
#endif
		successful = true;
	}
	else {
		userConfig.open(fullPath.c_str(), ios::out | ios::trunc);
		if(userConfig.good()) {
#ifdef _DEBUG
			std::clog << "schreibe Daten...\n";
#endif

			userConfig << FILE_COMMENT_CHAR << " === Spielereinstellungen ===\n";
			// write configs
			userConfig << CONFIG_PREFIX_TYPE << CONFIG_DELIMITER << playerList[0]->myChar->getType() << '\n';
			userConfig << CONFIG_PREFIX_NAME << CONFIG_DELIMITER << playerList[0]->getName() << '\n';
			userConfig << FILE_COMMENT_CHAR << " Spielerfarben: R G B\n";
			Color buffer;
			buffer = playerList[0]->myChar->getColor(AVATAR_COLOR_HAIR);
			userConfig << CONFIG_PREFIX_COL_HAIR << CONFIG_DELIMITER << buffer.r << " " << buffer.g << " " << buffer.b << '\n';
			buffer = playerList[0]->myChar->getColor(AVATAR_COLOR_SKIN);
			userConfig << CONFIG_PREFIX_COL_SKIN << CONFIG_DELIMITER << buffer.r << " " << buffer.g << " " << buffer.b << '\n';
			buffer = playerList[0]->myChar->getColor(AVATAR_COLOR_SHIRT);
			userConfig << CONFIG_PREFIX_COL_SHIRT << CONFIG_DELIMITER << buffer.r << " " << buffer.g << " " << buffer.b << '\n';
			buffer = playerList[0]->myChar->getColor(AVATAR_COLOR_GLOVES);
			userConfig << CONFIG_PREFIX_COL_GLOVES << CONFIG_DELIMITER << buffer.r << " " << buffer.g << " " << buffer.b << '\n';
			buffer = playerList[0]->myChar->getColor(AVATAR_COLOR_TROUSERS);
			userConfig << CONFIG_PREFIX_COL_TROUSERS << CONFIG_DELIMITER << buffer.r << " " << buffer.g << " " << buffer.b << '\n';
			buffer = playerList[0]->myChar->getColor(AVATAR_COLOR_BOOTS);
			userConfig << CONFIG_PREFIX_COL_BOOTS << CONFIG_DELIMITER << buffer.r << " " << buffer.g << " " << buffer.b << '\n';
			userConfig << FILE_COMMENT_CHAR << '\n';

			userConfig << FILE_COMMENT_CHAR << " === Grafik ===\n";
			int screenWidth, screenHeight;
			getScreenMetrics(&screenWidth, &screenHeight);
			userConfig << CONFIG_PREFIX_RES_WIDTH  << CONFIG_DELIMITER << screenWidth << '\n';
			userConfig << CONFIG_PREFIX_RES_HEIGHT << CONFIG_DELIMITER << screenHeight << '\n';

			userConfig << FILE_COMMENT_CHAR << " === Sonstiges ===\n";
			userConfig << CONFIG_PREFIX_LAST_IP << CONFIG_DELIMITER << IPAddress << '\n';
			int camAngle;
			if(camPos == VIEW_EYE_CAM)
				camAngle = CAM_EYE;
			else if(camPos == VIEW_HELI_CAM)
				camAngle = CAM_HELI;
			else
				camAngle = CAM_HEAD;
			userConfig << CONFIG_PREFIX_CAM << CONFIG_DELIMITER << camAngle << '\n';
			userConfig << CONFIG_PREFIX_HS_FREE << CONFIG_DELIMITER <<
				cryptHighscore((long)hsFreestyle.value, false) << ' ' <<
				cryptHighscore(hsFreestyle.name, false) << '\n';
			userConfig << CONFIG_PREFIX_HS_DOWN << CONFIG_DELIMITER <<
				cryptHighscore((long)(hsDownhill.value * 1000.0), false) << ' ' <<
				cryptHighscore(hsDownhill.name, false) << '\n';
			userConfig << CONFIG_PREFIX_HS_SLALOM << CONFIG_DELIMITER <<
				cryptHighscore((long)(hsSlalom.value * 1000.0), false) << ' ' <<
				cryptHighscore(hsSlalom.name, false) << '\n';
			userConfig << FILE_COMMENT_CHAR << " === Ende ===\n\n";

			configWasChanged = false;
			successful = true;
#ifdef _DEBUG
			std::clog << "fertig!";
#endif
		}
		else {
			std::cerr << "Fehler: kann die Datei \"" << fullPath << "\" nicht schreiben!";
			successful = false;
		}
		userConfig.close();
	}

#ifdef _DEBUG
	std::clog << "\n\n";
#endif

	// check, wether the resolution was changed
	//{
	//	int w, h;
	//	getScreenMetrics(&w, &h);
	//	if(w != currentResWidth || h != currentResHeight) {
	//		currentResWidth  = w;
	//		currentResHeight = h;
	//		//quitGame(0, true);
	//		MessageBox(NULL, "Sie müssen das Spiel neu starten, damit die Änderungen aktiv werden.",
	//			"OpenGL WinterGames 2005", MB_OK | MB_ICONINFORMATION);
	//		// HACK: Muss verbessert werden!
	//	}
	//}
	return successful;
}

std::string MountainRanger::cryptHighscore(std::string value, bool decode) {
	for(unsigned int i = 0; i < value.length(); i++) {
		value[i] += 128;
		value[i] %= 254;
	}
	return value;
}

long MountainRanger::cryptHighscore(long value, bool decode) {
	_ASSERT(sizeof(long) == 4);
	long bitMask = 0x15280537;
	value = value ^ bitMask;
	return value;
}

void MountainRanger::quitGame(int returnCode, bool restart) {
#ifdef _DEBUG
	std::cout << "\n\nSpeichere Einstellungen und beende...\n";
#endif

	saveConfig();

#ifdef _DEBUG
	std::cout << "Fertig! Ciao!\n";
#endif

#ifndef _DEBUG
	glutLeaveGameMode();
#endif
	//if(restart)
		//ShellExecute(NULL, NULL, MY_FILENAME, NULL, NULL, SW_SHOWNORMAL);
	exit(returnCode);
}

int compareFuncLong(const void* wert1, const void* wert2) {
	return (int)(*(long*)wert2 - *(long*)wert1);
}

int compareFuncDouble(const void* wert1, const void* wert2) {
	if(*(double*)wert1 == 0)
		return 1;
	else if(*(double*)wert2 == 0)
		return -1;
	else {
		double diff = *(double*)wert1 - *(double*)wert2;
		if(diff > 0.0)	// Problem: int-cast rundet bei diff < 1 falsch
			return 1;
		else
			return -1;
	}
}
