#pragma once

#ifdef _DEBUG
#define CRTDBG_MAP_ALLOC
#endif
#include <crtdbg.h>

#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include <shlobj.h>		// used to determine user folder for wintergames.cfg
#include <Iphlpapi.h>	// used to determine the local ip-addresses
//#include <shellapi.h>	// used to restart the game after changing the screen resolution
#include <windows.h>

#include "../particleLib/ParticleEmitter.h"
#include "Boarder.h"
#include "Carver.h"
#include "Avatar.h"
#include "Player.h"
#include "HumanPlayer.h"
#include "AIPlayer.h"
#include "LANPlayer.h"
#include "../coolTex/Bitmap.h"
#include "../menuLib/menu.h"
#include "../menuLib/credMenu.h"
#include "../menuLib/lobbyMenu.h"
#include "Matterhorn.h"
#include "../coolLib/Vector3D.h"
#include "../coolLib/Matrix3D.h"
#include "SlalomGate.h"
#include "Tree.h"
#include "Rock.h"
#include "stylePoints.h"
#include "../NetworkPackets/PlayerPacketProcessor.h"

#include "../NetworkPackets/NetworkPacket.h"
#include "../NetworkPackets/ProtSpeedAngle.h"
#include "../NetworkPackets/ProtHeading.h"
#include "../NetworkPackets/ProtPosition.h"
#include "../NetworkPackets/ProtCrash.h"
#include "../NetworkPackets/ProtStart.h"
#include "../NetworkPackets/ProtFinish.h"
#include "../NetworkPackets/ProtInitInformation.h"
#include "../NetworkPackets/ProtPlayerNum.h"
#include "../NetworkPackets/ProtCreatePlayers.h"
#include "../NetworkPackets/ProtDeletePlayers.h"
#include "../NetworkPackets/ProtGameType.h"
#include "../NetworkPackets/ProtStylePoints.h"
#include "../NetworkPackets/ProtFinalTime.h"
#include "../NetworkPackets/ProtLastFallSpeed.h"
#include "../NetworkPackets/ProtDayTime.h"
#include "../NetworkPackets/ProtReady.h"
#include "../NetworkPackets/ProtReadyPush.h"
#include "../NetworkPackets/ProtError.h"
#include "../NetworkPackets/ProtVersion.h"
#include "../NetworkPackets/ProtPing.h"
#include "../NetworkPackets/ProtLobby.h"

#include "../WinterGames/define.h"

using namespace std;

void menuCB(menu* calledMenu, unsigned int index);
void getScreenFlags(int* res, bool* wide);
void getScreenMetrics(int* w, int* h);
void changeResolution(int w, int h);

int compareFuncLong(const void* wert1, const void* wert2);
int compareFuncDouble(const void* wert1, const void* wert2);

//extern int displayWidth, displayHeight;
extern GLfloat colFogDay[], colFogNight[];
extern Color colBackgroundDay, colBackgroundNight;

typedef struct highscore_struct {
	double value;
	std::string name;
} highscore;

class MountainRanger
{
public:
	MountainRanger(void);
	~MountainRanger(void);

	void start();
	void update();
	void keyboardHandler(int key, int modifiers, bool isSpecial = true);
	void moveAll(float x, float y, float z);
	void rotateAll(float x, float y, float z);

	void processNetworkPacket(char* packet, unsigned int socketNum);
	bool packetsReady(unsigned int socketNum);
	char* getPacket(unsigned int socketNum);
	bool socketToClose();
	unsigned int getSocketToClose();
	bool createPlayer();
	void killPlayer(unsigned int socketNum);
	std::string getIPAddress();
	bool runClient();
	bool runServer();
	void networkError(unsigned int errorID);
	bool inline __fastcall gameIsRunning() const { return !(gameStatusFlags & GAME_STATUS_MENU); }
	void quitGame(int returnCode = 0, bool restart = false);

	// callback function for the menus
	void menuCallback(menu*, unsigned int);

#ifdef _DEBUG
	void printQuadsClipped();
	void leakReport();
#endif

protected:
	Matterhorn		*horn;
	ParticleEmitter	*pEmitter;

	std::vector<Player*>		playerList;
	std::vector<SlalomGate*>	gates;
	std::vector<Actor*>			obstacles;
	unsigned int	obstacleSeed;
	unsigned int	currentGateIndicatorDL, currentGate;
	float			playerStartAngle;

	void networkUpdate(char* packet, unsigned int playerNum);
	void sendSelfAndAI(float timeDiff);
	void sendInfoSpeedAngle(unsigned int playerNum);
	void sendInfoHeading(unsigned int playerNum);
	void sendInfoPosition(unsigned int playerNum);
	void sendInfoStylePoints(unsigned int playerNum);
	void sendInfoFinalTime(unsigned int playerNum);
	void sendInfoDayTime(unsigned int playerNum);
	void sendInfoReadyFlag(unsigned int playerNum);
	PlayerPacketProcessor	*packetProcessor;
	//char					*IPAddress;
	std::string				IPAddress;	// as string, the IP-"address" can be a hostname, too
	//std::string			getAIName(std::string name);
	bool kickPlayer(unsigned int playerNum);
	void updateAllPlayers(unsigned int start);
	void sendInit(unsigned int playerNum);
	void clearPlayerList();
	bool networkAllPlayersReady() const;

	clock_t			lastUpdate;
	//DWORD			lastUpdate;
	float			timeDiff;
	float			lastNetworkUpdateSpeedAngle;
	float			lastNetworkUpdateHeading;
	float			lastNetworkUpdatePosition;
	float			lastNetworkUpdateStylePoints;
	float			lastNetworkPing;
	bool			sendFinalTime;
	float			countdown;
	//float			localPlayerMaxHeight;
	std::vector<clock_t>		startTime;
	//std::vector<clock_t>		finishTime;
	std::vector<double>			finalTime;
	std::vector<stylePoints*>	style;

	unsigned int	gameStatusFlags;
	menu		mainMenu;
	lobbyMenu*	lanLobby;
	menuEntry*	keyboardTarget;
	std::string	keyboardInput, keyboardTargetDefault, keyboardTargetOldValue;
	bool		configWasChanged;
	highscore	hsFreestyle, hsDownhill, hsSlalom;
	int			currentResWidth, currentResHeight;

	int			camPlayerNum;	// nicht unsigned, damit man rückwärts mit "cpn--" durchlaufen kann
	clock_t		lastPlayerViewChange;
	Vector3D*	height;
	Vector3D*	speed;
	Vector3D*	wind;
	Vector3D*	movement;
	Vector3D*	rotation;
	Vector3D	targetRotation;
	Vector3D	camPos;
	Vector3D	camDisplacement;
	Vector3D	menuRot;
	Vector3D	menuCam;
	Color		HUDcol;
	unsigned int* quadRenderList;	// TODO: werden die beiden noch gebraucht?
	unsigned int* quadRenderNumber;

	void buildMainMenu();
	void activateMainMenu();
	void activateGameMode();
	void addAIPlayer(unsigned int number = 1);
	unsigned int addLanPlayer();
	void setObstacles(unsigned int number = OBSTACLE_NUM);
	void movePlayersToStartPositions(float startAngle = -1.0f);
	void updatePlayers();
	void renderHUD();
	void updateLigthing();
	void setNightFlag();
	void turnPlayer(float angle, unsigned int index = 0);
	void accPlayer(float factor, unsigned int index = 0);
	void setCamera(int newCamIndex);
	void createSlalomRoute(unsigned int length = SLALOM_ROUTE_LENGTH);
	void checkGates();

	void loadConfig();
	bool saveConfig();
	std::string	cryptHighscore(std::string value, bool decode = true);
	long		cryptHighscore(long value, bool decode = true);

	int doParticleUpdate;

private:

};
