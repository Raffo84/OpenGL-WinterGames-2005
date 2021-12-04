
#ifndef wintergames_h_
#define wintergames_h_

#include <iostream>
#include <winsock2.h>
#include <shlobj.h>
#include <sstream>
#include <string>
#include <ctime>
#include <vector>
#include "../coolLib/GL/glut.h"

#include "define.h"

#include "../coolLib/Shape3D.h"
#include "../coolLib/Color.h"
#include "../mountainLib/MountainRanger.h"
#include "NetworkIO.h"
//#include "Models.h"

// UI
int displayWidth, displayHeight;
//bool isWidescreen = false;
//bool isFullscreen = false;
//int screenResolution = RESOLUTION_LOW;
int mouseX, mouseY;
bool leftButtonPressed = false, rightButtonPressed = false, middleButtonPressed = false;
bool showFrameCounter = false;

Vector3D camPos;
Vector3D camLookTo;
Vector3D camUp;

clock_t fpsLastTime;
unsigned int fpsCount, fpsRate;

//global variables:
MountainRanger* mRanger;
int keyDown[256];
int keyBitMask[256];
int specialDown[256];
int specialBitMask[256];

GLfloat LightPosition[] = {-300.0f, 4500.0f, 300.0f, 0.0f};
//#ifndef NIGHTLY_BUILD
GLfloat LightDayAmbient[]	= {0.4f, 0.4f, 0.4f, 1.0f};
GLfloat LightDayDiffuse[]	= {0.5f, 0.5f, 0.4f, 1.0f};
GLfloat colFogDay[]			= {0.9f, 0.9f, 0.9f, 0.5f};
Color colBackgroundDay		= Color(0.9f, 0.9f, 0.9f);
//#else
GLfloat LightNightAmbient[]	= {0.0f, 0.0f, 0.2f, 1.0f};
GLfloat LightNightDiffuse[]	= {0.2f, 0.2f, 0.4f, 1.0f};
GLfloat colFogNight[]		= {0.1f, 0.0f, 0.2f, 0.5f};
Color colBackgroundNight	= Color(0.05f, 0.0f, 0.2f);
//#endif

//bool introRunning = true;
//clock_t introTimer, introStart;

// function forward declarations
void rotateObject(unsigned int index, int phi, int theta, int eta);
void moveObject(unsigned int index, float x, float y, float z);
void rotateScene(float phi, float theta, float eta);
void moveScene(int x, int y, int z);
void setCamera();
void init();

void stepIntro();

// network
NetworkIO *network;


#endif // wintergames_h_
