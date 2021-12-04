
#include "WinterGames.h"

void display() {
	if(clock() - fpsLastTime >= CLK_TCK) {
		fpsLastTime = clock();
		fpsRate = fpsCount;
		fpsCount = 0;
	}
	fpsCount++;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	mRanger->update();

	// FPS
	if(showFrameCounter) {
		stringstream oss;
		string fpsText;
		oss << "FPS: " << fpsRate;
		fpsText = oss.str();
		glColor3f(0.0f, 0.0f, 0.0f);
		glRasterPos3f(1.0f, 1.0f, -3.0f);
		GLboolean isEnTex = glIsEnabled(GL_TEXTURE_2D);
		GLboolean isEnLight = glIsEnabled(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		for(unsigned int i = 0; i < fpsText.length(); i++) {
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13, (int)fpsText[i]);
		}
		if(isEnTex)		glEnable(GL_TEXTURE_2D);
		if(isEnLight)	glEnable(GL_LIGHTING);
	}

#ifdef SHOW_ZERO
	// Nullpunkt
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3i(-20, 0, 0);
	glVertex3i( 20, 0, 0);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3i(0, -20, 0);
	glVertex3i(0,  20, 0);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3i(0, 0, -20);
	glVertex3i(0, 0,  20);
	glEnd();
#endif

	glutSwapBuffers();
}

void reshape(int w, int h) {
	displayWidth  = w;
	displayHeight = h;
	glViewport(0, 0, displayWidth, displayHeight);

	setCamera();
}

void getScreenFlags(int* res, bool* wide) {
	if(displayWidth == RESOLUTION_LOW_WIDTH && displayHeight == RESOLUTION_LOW_HEIGHT) {
		*res = RESOLUTION_LOW;
		*wide = false;
	}
	else if(displayWidth == RESOLUTION_LOW_WIDTH_W && displayHeight == RESOLUTION_LOW_HEIGHT_W) {
		*res = RESOLUTION_LOW;
		*wide = true;
	}
	else if(displayWidth == RESOLUTION_MED_WIDTH && displayHeight == RESOLUTION_MED_HEIGHT) {
		*res = RESOLUTION_MED;
		*wide = false;
	}
	else if(displayWidth == RESOLUTION_MED_WIDTH_W && displayHeight == RESOLUTION_MED_HEIGHT_W) {
		*res = RESOLUTION_MED;
		*wide = true;
	}
	else if(displayWidth == RESOLUTION_HI_WIDTH && displayHeight == RESOLUTION_HI_HEIGHT) {
		*res = RESOLUTION_HI;
		*wide = false;
	}
	else if(displayWidth == RESOLUTION_HI_WIDTH_W && displayHeight == RESOLUTION_HI_HEIGHT_W) {
		*res = RESOLUTION_HI;
		*wide = true;
	}
	else {
		*res = RESOLUTION_CUSTOM;
		*wide = false;
	}
	//*res  = screenResolution;
	//*wide = isWidescreen;
}

void getScreenMetrics(int* w, int* h) {
	*w = displayWidth;
	*h = displayHeight;
}

void changeResolution(int w, int h) {
	displayWidth  = w;
	displayHeight = h;
	//reshape(w, h);
}

void changeResolution(int resFlag, bool wide) {
	// TODO: Avoid this function and remove it
	int width, height;
	//isWidescreen = wide;
	switch(resFlag) {
	case RESOLUTION_HI:
		if(false) {	//isWidescreen) {
			height = RESOLUTION_HI_HEIGHT_W;
			width  = RESOLUTION_HI_WIDTH_W;
		}
		else {
			height = RESOLUTION_HI_HEIGHT;
			width  = RESOLUTION_HI_WIDTH;
		}
		break;
	case RESOLUTION_MED:
		if(false) {	//isWidescreen) {
			height = RESOLUTION_MED_HEIGHT_W;
			width  = RESOLUTION_MED_WIDTH_W;
		}
		else {
			height = RESOLUTION_MED_HEIGHT;
			width  = RESOLUTION_MED_WIDTH;
		}
		break;
	case RESOLUTION_LOW:
	default:
		if(false) {	//isWidescreen) {
			height = RESOLUTION_LOW_HEIGHT_W;
			width  = RESOLUTION_LOW_WIDTH_W;
		}
		else {
			height = RESOLUTION_LOW_HEIGHT;
			width  = RESOLUTION_LOW_WIDTH;
		}
		break;
	}
	if(displayWidth != width || displayHeight != height) {
		//screenResolution = resFlag;
#ifdef _DEBUG
		glutReshapeWindow(width, height);
		//if(isFullscreen)
			//glutFullScreen();
#else
		stringstream gameMode;
		gameMode << width << "x" << height << ":32@60";
		glutLeaveGameMode();
		//glutGameModeString(gameMode.str().c_str());
		//glutEnterGameMode();
#endif
	}
}

void rotateScene(float phi, float theta, float eta) {
#ifdef _DEBUG
	std::cout << "rotateScene(" << phi << ", " << theta << ", " << eta << ");\n";
#endif

	float myPhi, myTheta, myEta;
	myPhi   = phi   * 0.3f;
	myTheta = theta * 0.3f;
	myEta   = eta   * 0.3f;

	if(phi != 0 || theta != 0) {
		// phi entspricht vor/zurück kippen
		// theta entspricht links/rechts drehen
		Matrix3D mat = Matrix3D();
		mat.createRotationMatrix(0.0f, -myTheta, 0.0f);
		camLookTo = mat * camLookTo;
		camLookTo.normalize();
		mat.createRotationMatrix(myPhi, 0.0f, 0.0f);
		camLookTo = mat * camLookTo;
		camLookTo.normalize();
	}
	if(eta != 0) {
	}

	setCamera();
}

void rotateObject(unsigned int index, int phi, int theta, int eta) {
}

void moveScene(int x, int y, int z) {
#ifdef _DEBUG
	std::cout << "move(" << x << ", " << y << ", " << z << ");\n";
#endif

	if(z != 0) {
		// z Koordinate entspricht vor/zurück
		camPos.x += (z * camLookTo.x);
		camPos.y += (z * camLookTo.y);
		camPos.z += (z * camLookTo.z);
	}
	if(x != 0) {
		// x Koordinate entspricht links/rechts
		Vector3D tmp = Vector3D(camLookTo, camUp);
		tmp.normalize();
		camPos.x += (x * tmp.x);
		camPos.y += (x * tmp.y);
		camPos.z += (x * tmp.z);
	}
	if(y != 0) {
		// y Koordinate entspricht hoch/runter
		camPos.x += (y * camUp.x);
		camPos.y += (y * camUp.y);
		camPos.z += (y * camUp.z);
	}

	setCamera();
}

//void moveObject(unsigned int index, float x, float y, float z) {
//}

void keyboard (unsigned char key, int x, int y) {
	int modi = glutGetModifiers();
	switch(key) {
#ifdef _DEBUG
	case 'w':	// Wireframe ON
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 'q':	// Wireframe OFF
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 'm':	// printQuadsClipped
		mRanger->printQuadsClipped();
		break;
	case 'l':	// print memory leaks
		mRanger->leakReport();
		break;
#endif
	case 6:		// Strg+F
		if(modi & GLUT_ACTIVE_CTRL) {
			if(showFrameCounter)
				showFrameCounter = false;
			else
				showFrameCounter = true;
		}
		// no break, pass to Ranger!
	default:
		keyDown[key] = 1;
		keyBitMask[key] = modi;
		if (!mRanger->gameIsRunning()) mRanger->keyboardHandler((int)key, modi, false);
		break;
	}
}

void keyboardUp (unsigned char key, int x, int y) {
	keyDown[key] = 0;
}

void specialKey(int key, int x, int y) {
	int modi = glutGetModifiers();
	switch (key)
    {
#ifdef _DEBUG
    case GLUT_KEY_F5:
		// strafing left
		moveScene(1, 0, 0);
		break;
	case GLUT_KEY_F8:
		// strafing right
		moveScene(-1, 0, 0);
		break;
	case GLUT_KEY_F6:
		// moving forward
		moveScene(0, 0, 1);
		break;
	case GLUT_KEY_F7:
		// moving backward
		moveScene(0, 0, -1);
		break;
#else
#pragma warning (disable: 4065)
#endif
	default:
		specialDown[key] = 1;
		specialBitMask[key] = modi;
		if (!mRanger->gameIsRunning()) mRanger->keyboardHandler(key, modi);
		break;
	}
#ifndef _DEBUG
#pragma warning (default: 4065)
#endif
}

void specialKeyUp(int key, int x, int y) {
	specialDown[key] = 0;
}

void mouse(int button, int state, int x, int y) {
	mouseX = x;
	mouseY = y;
	switch(button) {
	case GLUT_LEFT_BUTTON:
		if(state == GLUT_DOWN) {
			leftButtonPressed = true;				
		}
		else if (state == GLUT_UP) {
			leftButtonPressed = false;
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if(state == GLUT_DOWN) {
			rightButtonPressed = true;				
		}
		else if (state == GLUT_UP) {
			rightButtonPressed = false;
		}
		break;
	case GLUT_MIDDLE_BUTTON:
			if(state == GLUT_DOWN) {
				middleButtonPressed = true;
			}
			else if (state == GLUT_UP) {
				middleButtonPressed = false;
			}
			break;
	default:
		break;
	}
}

void mouseMotion(int x, int y) {
#ifdef _DEBUG
	if(rightButtonPressed)
		rotateScene((float)y-mouseY, (float)x-mouseX, 0.0f);
#endif

	mouseX = x;
	mouseY = y;
}

void menuCB(menu* calledMenu, unsigned int index) {
	mRanger->menuCallback(calledMenu, index);
}

//void stepIntro() {
//	unsigned int introDiff;
//	introDiff = clock() - introTimer;	// Just one call to clock() to avoid errors
//	introTimer += introDiff;	// == clock()
//	float phi, theta;
//	float x, y, z;
//	phi = theta = 0.0f;
//	x = y = z = 0.0f;

	//if(introTimer <= (INTRO_LENGTH - INTRO_ENDING)) {
		//phi = (-150.0f * (float)introDiff) / (float)(INTRO_LENGTH - INTRO_ENDING);
		//theta = (1050.0f * (float)introDiff) / (float)(INTRO_LENGTH - INTRO_ENDING);

		//y = (1250.0f * (float)introDiff) / (float)(INTRO_LENGTH - INTRO_ENDING);
	//}
	//else {
		//y = (1250.0f * (float)introDiff) / (float)(INTRO_ENDING);
	//}

	//rotateScene(phi, theta, 0.0f);
	//moveScene((int)x, (int)y, (int)z);

//	if(introTimer - introStart >= INTRO_LENGTH) {
//		introRunning = false;
//#ifdef _DEBUG
//		std::cout << "Intro beendet.\n";
//#endif
//		return;
//	}
//}

void idle() {
	network->update();
	
	//if(introRunning)
	//	stepIntro();
	if (mRanger->gameIsRunning()) {
		for (unsigned int i = 0; i < 256; i++) {
			if (keyDown[i] == 1) mRanger->keyboardHandler((int)i, keyBitMask[i], false);
		}
		for (unsigned int i = 0; i < 256; i++) {
			if (specialDown[i] == 1) mRanger->keyboardHandler((int)i, specialBitMask[i]);
		}
	}
	display();
}

void setCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)displayWidth/(GLfloat)displayHeight, 1.0f, 200.0f);
	gluLookAt(	camPos.x, camPos.y, camPos.z,
				camPos.x + camLookTo.x, camPos.y + camLookTo.y, camPos.z + camLookTo.z,
				camUp.x, camUp.y, camUp.z);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init() {
	std::cout << "Lade...\n";

	// Preloading Objects
	std::cout << "\tObjekte...\n";
	mRanger = new MountainRanger();

#ifdef _DEBUG
	glutReshapeWindow(displayWidth, displayHeight);
#else
	stringstream gameMode;
	gameMode << displayWidth << "x" << displayHeight << ":32@60";
	glutGameModeString(gameMode.str().c_str());
	glutEnterGameMode();
#endif

	// +++ OpenGL SETTINGS +++
	std::cout << "\tEinstellungen...\n";
	glShadeModel(GL_SMOOTH);
	glClearColor(colBackgroundDay.r, colBackgroundDay.g, colBackgroundDay.b, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK,  GL_FILL);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);	// Anti-Aliasing
	//glEnable(GL_POLYGON_SMOOTH);	// Strange rendering Error on GeForce4 MX 460
	glLineWidth(1.5f);

	// +++ TEXTURES +++
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);

	// +++ CAMERA +++
	std::cout << "\tKamera...\n";
	camPos		= CAM_POS;
	camLookTo	= CAM_LOOK_TO;
	camUp		= CAM_UP;
	setCamera();

	// +++ LIGHTS +++
	std::cout << "\tLichter & Nebel...\n";
	glLightfv(GL_LIGHT1, GL_AMBIENT,  LightDayAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE,  LightDayDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
	glLightfv(GL_LIGHT2, GL_AMBIENT,  LightNightAmbient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE,  LightNightDiffuse);
	glLightfv(GL_LIGHT2, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);

	// +++ FOG +++
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogfv(GL_FOG_COLOR, colFogDay);
	glFogf(GL_FOG_DENSITY, 0.4f);
	glHint(GL_FOG_HINT, GL_DONT_CARE);
	glFogf(GL_FOG_START, 60.0f);
	glFogf(GL_FOG_END, 180.0f);
	glEnable(GL_FOG);

	mRanger->start();
	// done.
	std::cout << "Fertig!\n\n";

#ifdef SHOW_FPS
	showFrameCounter = true;
#endif
}

int main (int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

#ifdef _DEBUG
	// normal Window, that the Console is still visible
	glutInitWindowPosition(350, 50);
	glutInitWindowSize(RESOLUTION_LOW_WIDTH, RESOLUTION_LOW_HEIGHT);
	glutCreateWindow(MENU_TITLE_MAIN);
//#else
	//stringstream gameMode;
	//gameMode << RESOLUTION_MED_WIDTH << "x" << RESOLUTION_MED_HEIGHT << ":32@60";
	//glutGameModeString(gameMode.str().c_str());
	//glutEnterGameMode();
#endif

	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	memset(keyDown, 0, sizeof(keyDown));
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutSpecialFunc(specialKey);
	glutSpecialUpFunc(specialKeyUp);
#ifdef _DEBUG
	// No mouse needed in release version
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	//glutPassiveMotionFunc(mouseMotion);
#endif
	glutIdleFunc(idle);

#ifndef _DEBUG
	glutSetCursor(GLUT_CURSOR_NONE);
#endif
	// network
	network = new NetworkIO(mRanger);

	glutMainLoop();

	return 0;
}

