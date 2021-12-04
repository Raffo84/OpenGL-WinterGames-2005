#include "AIPlayer.h"

AIPlayer::AIPlayer(void) {
	init();

	// Zufallsname aus Liste laden
	fstream file;
	file.open(AI_NAMELIST_FILE, ios_base::in);
	if(file.good()) {
		char* buffer = new char[FILE_LINE_MAX];
		std::vector<std::string> namelist;
		while(!file.eof()) {
			file.getline(buffer, FILE_LINE_MAX);
			if(buffer[0] == '\0' || buffer[0] == FILE_COMMENT_CHAR)
				continue;	// ignore empty or comment lines
			std::string newName = buffer;
			namelist.push_back(newName);
		}
		unsigned int temp = (unsigned int)namelist.size();
		unsigned int idx = rand() % (unsigned int)namelist.size();
		name = namelist[idx];
	}
	else {
		std::cerr << "Fehler! Kann die Datei \"" << AI_NAMELIST_FILE << "\" nicht oeffnen!\n";
		name = "Bot";
	}
	file.close();
}

AIPlayer::AIPlayer(Player* copy)
	: Player(copy) {
		isSet = true;
		oldSpeed = speed;
		oldAcc = heading.y;
		zeroSpeedCheck = 0;
		currentRot = 0.0f;
		rotSign = 1;
		waitForTurboBoost = AI_MAX_WAIT_FOR_TURBOBOOST / 10.0f;
		nameCol = NULL;
		myPoints = NULL;
}

AIPlayer::AIPlayer(std::string newName) {
	init();
	name = newName;
}

void AIPlayer::init() {
	//srand((unsigned int)time(NULL));
	speed =		3.0f;
	oldSpeed =	3.0f;
	oldAcc =	0.0f;
	lastHeight = 0.0f;
	toRotate =	0.0f;
	zeroSpeedCheck = 0;
	waitForTurboBoost = AI_MAX_WAIT_FOR_TURBOBOOST / 2.0f;

	//// Altes System: Bots wurden direkt zufällig plaziert
	//float startX = AI_MIN_START_X + ((float)rand() / (float)RAND_MAX) * (AI_MAX_START_X - AI_MIN_START_X);
	//float startZ = AI_MIN_START_Z + ((float)rand() / (float)RAND_MAX) * (AI_MAX_START_Z - AI_MIN_START_Z);
	//pos = Vector3D(startX, 0.0f, startZ);

	//heading = Vector3D(-1.0f, 0.0f, 0.0f);
	//float angle = -heading.getAngle(Vector3D(pos.x, 0.0f, pos.z));
	//turnPlayer(angle);
	//angle = -heading.getAngle(Vector3D(pos.x, 0.0f, pos.z));
	//if(angle < -0.5f)	// turned the wrong direction, now correct this
		//turnPlayer(angle);

	pos = Vector3D(0.0f, 0.0f, 0.0f);

	heading = Vector3D(-1.0f, 0.0f, 0.0f);

	isSet = false;
	currentRot = 0.0f;
	if(rand() > (RAND_MAX / 2))
		rotSign = 1;
	else
		rotSign = -1;

	if(rand() > (RAND_MAX / 2))
		switchPlayerChar();
	myChar->randomizeColors();
	nameCol = NULL;
	myPoints = NULL;
	lanReady = true;
}

AIPlayer::~AIPlayer(void) {
}

void AIPlayer::setGameMode(int mode) {
	gameMode = mode;

	// War ursprünglich nur bei Downhill, ist aber allgemein besser, deshalb --> init()
	//if(gameMode & AI_GAME_TYPE_DOWN) {
		//float angle = -heading.getAngle(Vector3D(pos.x, 0.0f, pos.z));
		//turnPlayer(angle);
//#ifdef _DEBUG
		//std::clog << '\t' << name << ": Drehe wegen Downhill um " << angle << " Grad.\n";
//#endif
	//}
}
void AIPlayer::update(float timeDiff, Vector3D normal) {
	if(!stopped && standUpTime <= 0.0f) {
		/* TODO: KI verbessern...
		 * Bot sollte schon gegendrehen, wenn er weniger beschleunigt,
		 * als letztes Mal. Problem dahinter: Unterschiedlich lange
		 * Frames bedeuten unterschiedliche Werte bei Geschwindigkeitsdifferenzen
		 */
		//if(oldSpeed > speed && currentRot > 1.0f) {
		if(oldAcc < heading.y && currentRot > 1.0f) {
#ifdef _DEBUG
			std::clog << name << ": getting slower...\n";
#endif
			rotSign *= -1;
			currentRot = 0.0f;
		}
		oldSpeed = speed;
		oldAcc = heading.y;

		if(gameMode & AI_GAME_TYPE_FREE) {
			if(waitForTurboBoost <= 0.0f) {
				jump();
				waitForTurboBoost = AI_MAX_WAIT_FOR_TURBOBOOST * ((float)rand() / (float)RAND_MAX);
			}
			waitForTurboBoost -= timeDiff;
		}

		float angle = timeDiff * rotSign * AI_ROTATION_ANGLE;
		if((gameMode & AI_GAME_TYPE_FREE) && lastHeight > 40.0f && toRotate == 0.0f)
			toRotate = 365.0f;
		if(toRotate > 0.0f) {
			angle /= AI_ROTATION_ANGLE;
			angle *= ROTATION_RATE;
			toRotate -= angle;
			if(toRotate < 0.0f || speed < 2.8f)
				toRotate = 0.0f;
		}
		bool moveOn = isFlying() && (gameMode & AI_GAME_TYPE_FREE);
		currentRot += angle;
		if((abs(currentRot) > abs(AI_MAX_ROTATION)) && !moveOn) {
			if(zeroSpeedCheck == 0) {
				zeroSpeedCheck++;
				rotSign *= -1;
			}
			else {
				// we haven't moved since last rotation change. just go MAX_ROTATION further...
				currentRot = 0.0f;
#ifdef _DEBUG
				std::clog << name << ": stuck, moving on...\n";
#endif
				if(zeroSpeedCheck >= 4) {
					// After a full rotation without effect, a little cheating
					speed = 1.0f;
					move(0.0f, 1.0f, 0.0f);
#ifdef _DEBUG
					std::clog << name << ": still not moving, resetting...\n";
#endif
				}
				zeroSpeedCheck++;
			}
		}
		if(zeroSpeedCheck && speed > 0.0f)
			zeroSpeedCheck = 0;
		turnPlayer(angle);
		myPoints->incAngle(angle);
	}
	Player::update(timeDiff, normal);
}

void __fastcall AIPlayer::fall(float timeDiff, float height) {
	if(!isSet) {
		pos.y -= height;
		height = 0.0f;
		isSet = true;
	}
	lastHeight = height;
	Player::fall(timeDiff, height);
}

void AIPlayer::render() const {
	glPushMatrix();
	GLboolean isTexEn = glIsEnabled(GL_TEXTURE_2D);
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
	if(isTexEn) glEnable(GL_TEXTURE_2D);
	glPopMatrix();

	Player::render();
}

void __fastcall AIPlayer::stop(float delay) {
	Player::stop(delay);
	oldSpeed = speed;
	lanReady = true;
}

void __fastcall AIPlayer::restart(float delay) {
	Player::restart(delay);
	oldSpeed = speed;
	oldAcc = heading.y;
	toRotate = 0.0f;
}
