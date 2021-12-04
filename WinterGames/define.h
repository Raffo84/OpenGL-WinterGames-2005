
#pragma once
#ifndef define_h_
#define define_h_
// lieber zweimal ausschlieﬂen, als gar nicht... ;)

// -=- GENERAL -=-
#define PI 3.1415926535897932384626433f

#define CURRENT_VERSION	6
#define MINIMUM_VERSION	6

#ifdef _DEBUG
#define SHOW_FPS
#define SHOW_ZERO
#endif

#define HUD_COORD_X		-1.4f
#define HUD_COORD_Y		 0.9f
#define HUD_COORD_Z		-3.0f
#define HUD_COLOR_R		 0.0f
#define HUD_COLOR_G		 0.0f
#define HUD_COLOR_B		 0.4f
#define HUD_COLOR_R_N	 0.0f
#define HUD_COLOR_G_N	 0.5f
#define HUD_COLOR_B_N	 1.0f
#define FREESTYLE_DURATION		90.0f	// seconds
#define STAND_UP_DELAY			3.0f	// seconds
#define CAM_MAX_ROTATION_RATE	60.0f	// degrees per second
#define CAM_DISPLACEMENT_MIN_SPEED	8.3f	// meters per second; ~30 km/h
#define CAM_DISPLACEMENT_STEP	13.9f	// difference to CD_MIN_SPEED in m/s; ~50 km/h
#define ROTATION_RATE			90.0f	// degrees per second
#define BRAKE_RATE				5.0f	// meters/(second*second)

// +++ GAME STATUS FLAGS +++
#define GAME_STATUS_NONE		0x0
#define GAME_STATUS_MENU		0x1
#define GAME_STATUS_MENU_PLAYER	0x2
#define GAME_STATUS_TYPE_DOWN	0x4
#define GAME_STATUS_TYPE_SLALOM	0x8
#define GAME_STATUS_TYPE_FREE	0x10
#define GAME_STATUS_TYPE_ALL	(GAME_STATUS_TYPE_FREE | GAME_STATUS_TYPE_SLALOM | GAME_STATUS_TYPE_DOWN)
#define GAME_STATUS_LANGAME		0x20
#define GAME_STATUS_LAN_WAITING	0x40
#define GAME_STATUS_SERVER		0x80
#define GAME_STATUS_LAN_READY	0x400
#define GAME_STATUS_LAN_RUNNING	0x1000
#define GAME_STATUS_LAN_ALL		(GAME_STATUS_LANGAME | GAME_STATUS_LAN_WAITING | GAME_STATUS_SERVER | GAME_STATUS_LAN_READY | GAME_STATUS_LAN_RUNNING)
#define GAME_STATUS_FINISHED	0x100
#define GAME_STATUS_COUNTDOWN	0x200
#define GAME_STATUS_NIGHT		0x800
#define GAME_STATUS_DAY			0x0
#define GAME_STATUS_UNUSED_4	0x2000
#define GAME_STATUS_UNUSED_5	0x4000
#define GAME_STATUS_UNUSED_6	0x8000

// -=- TEXTURES -=-
#ifdef _DEBUG
#define TEXTURE_SNOW_FILE		"../textures/snow.bmp"
#define TEXTURE_ROCK_FILE		"../textures/rock.bmp"
#define TREE_TEXTURE_FILE		"../textures/tree.bmp"
//#define TEXTURE_SNOWBOARD_FILE	"../textures/snowboard.bmp"
//#define TEXTURE_GRASS_FILE		"../textures/grass.bmp"
#else
#define TEXTURE_SNOW_FILE		"textures/snow.bmp"
#define TEXTURE_ROCK_FILE		"textures/rock.bmp"
#define TREE_TEXTURE_FILE		"textures/tree.bmp"
//#define TEXTURE_SNOWBOARD_FILE	"textures/snowboard.bmp"
#endif
#define TEXTURE_GRASS_FILE		TREE_TEXTURE_FILE

// -=- OTHER EXTERNAL FILES -=-
#define AI_NAMELIST_FILE			"namelist.txt"
#define USER_CONFIG_FILE			"wintergames.cfg"
#define FILE_LINE_MAX				200
#define FILE_COMMENT_CHAR			'#'

#define CONFIG_DELIMITER			'='
#define CONFIG_PREFIX_TYPE			"Char"
#define CONFIG_PREFIX_NAME			"Name"
#define CONFIG_PREFIX_LAST_IP		"IP"
#define CONFIG_PREFIX_COL_HAIR		"Haarfarbe"
#define CONFIG_PREFIX_COL_SKIN		"Hautfarbe"
#define CONFIG_PREFIX_COL_SHIRT		"Jacke"
#define CONFIG_PREFIX_COL_GLOVES	"Handschuhe"
#define CONFIG_PREFIX_COL_TROUSERS	"Hose"
#define CONFIG_PREFIX_COL_BOOTS		"Schuhe"
#define CONFIG_PREFIX_HS_FREE		"Freestyle"
#define CONFIG_PREFIX_HS_DOWN		"Downhill"
#define CONFIG_PREFIX_HS_SLALOM		"Slalom"
#define CONFIG_PREFIX_CAM			"Cam"
#define CONFIG_PREFIX_RES_WIDTH		"Breite"
#define CONFIG_PREFIX_RES_HEIGHT	"Hoehe"

// -=- MISC. GRAPHICS -=-
#ifdef _DEBUG
#define GLOBAL_LEVEL_OF_DETAIL	10.0f
#else
#define GLOBAL_LEVEL_OF_DETAIL	20.0f
#endif

#define RESOLUTION_CUSTOM	0x0
#define RESOLUTION_LOW		0x1
#define RESOLUTION_MED		0x2
#define RESOLUTION_HI		0x4
//#define RESOLUTION_LOW_WIDTH	800
//#define RESOLUTION_LOW_HEIGHT	600
//#define RESOLUTION_LOW_WIDTH_W	960
//#define RESOLUTION_LOW_HEIGHT_W	600
//#define RESOLUTION_MED_WIDTH	1024
//#define RESOLUTION_MED_HEIGHT	768
//#define RESOLUTION_MED_WIDTH_W	1280
//#define RESOLUTION_MED_HEIGHT_W	800
//#define RESOLUTION_HI_WIDTH		1280
//#define RESOLUTION_HI_HEIGHT	1024
//#define RESOLUTION_HI_WIDTH_W	1680
//#define RESOLUTION_HI_HEIGHT_W	1050

// OK, minor update to reflect modern monitors... - RS, 2021-12-04
#define RESOLUTION_LOW_WIDTH	1920
#define RESOLUTION_LOW_HEIGHT	1080
#define RESOLUTION_LOW_WIDTH_W	2560
#define RESOLUTION_LOW_HEIGHT_W	1080
#define RESOLUTION_MED_WIDTH	2560
#define RESOLUTION_MED_HEIGHT	1440
#define RESOLUTION_MED_WIDTH_W	3440
#define RESOLUTION_MED_HEIGHT_W	1440
#define RESOLUTION_HI_WIDTH		3840
#define RESOLUTION_HI_HEIGHT	2160
#define RESOLUTION_HI_WIDTH_W	3840
#define RESOLUTION_HI_HEIGHT_W	1600

#define CAM_EYE			0x1
#define CAM_HEAD		0x2
#define CAM_HELI		0x4
#define VIEW_EYE_CAM	Vector3D(0.0f, 1.5f, 5.0f)
#define VIEW_HEAD_CAM	Vector3D(0.0f, 3.0f, 10.0f)
#define VIEW_HELI_CAM	Vector3D(0.0f, 6.0f, 20.0f)
#define CAM_MIN_HEIGHT	0.5f
#define CAM_POS			Vector3D(0.0f, 0.0f,  0.0f)
#define CAM_LOOK_TO		Vector3D(0.0f, 0.0f, -1.0f);
#define CAM_UP			Vector3D(0.0f, 1.0f, 0.0f);

// -=- NETWORK -=-
#define PORT			2143
#define BUFFERSIZE		8192
#define MAX_PLAYERS		16
#define MAX_IP_LENGTH	15
#define NETWORK_UPDATE_SPEED_ANGLE		0.05f
#define NETWORK_UPDATE_POSITION			0.2f
#define NETWORK_UPDATE_HEADING			0.2f
#define NETWORK_UPDATE_STYLE_POINTS		0.5f
#define NETWORK_PING_SEND_TIME			3.0f
#define NETWORK_PING_RECV_TIME			15.0f

// -+- network error IDs -+-
#define CLIENT__SERVER_NOT_FOUND			1
#define SERVER__CLIENT_WRONG_VERSION		2
#define SERVER__CLIENT_KICKED				3
#define SERVER__SERVER_FULL					4
#define SERVER_CLIENT__TIME_OUT				5

// -+- protocol IDs -+-
#define PROT_SPEED_ANGLE_ID 'A'
#define PROT_SPEED_ANGLE_LENGTH 12
#define PROT_POSITION_ID 'B'
#define PROT_POSITION_LENGTH 16
#define PROT_CRASH_ID 'C'
#define PROT_CRASH_LENGTH 9
#define PROT_START_ID 'D'
#define PROT_START_LENGTH 8
#define PROT_FINISH_ID 'E'
#define PROT_FINISH_LENGTH 8
#define PROT_INIT_INFORMATION_ID 'F'
#define PROT_INIT_INFORMATION_LENGTH 80 + MAX_NAME_LENGTH
#define PROT_PLAYER_NUM_ID 'G'
#define PROT_PLAYER_NUM_LENGTH 4
#define PROT_PLAYER_DROP_ID 'H'
#define PROT_PLAYER_DROP_LENGTH 4
#define PROT_CREATE_PLAYERS_ID 'I'
#define PROT_CREATE_PLAYERS_LENGTH 8
#define PROT_DELETE_PLAYERS_ID 'J'
#define PROT_DELETE_PLAYERS_LENGTH 8
#define PROT_GAME_TYPE_ID 'K'
#define PROT_GAME_TYPE_LENGTH 8
#define PROT_STYLE_POINTS_ID 'L'
#define PROT_STYLE_POINTS_LENGTH 8
#define PROT_FINAL_TIME_ID 'M'
#define PROT_FINAL_TIME_LENGTH 12
#define PROT_LAST_FALL_SPEED_ID 'N'
#define PROT_LAST_FALL_SPEED_LENGTH 8
#define PROT_DAY_TIME_ID 'O'
#define PROT_DAY_TIME_LENGTH 8
#define PROT_READY_ID 'P'
#define PROT_READY_LENGTH 8
#define PROT_ERROR_ID 'Q'
#define PROT_ERROR_LENGTH 8
#define PROT_VERSION_ID 'R'
#define PROT_VERSION_LENGTH 8
#define PROT_PING_ID 'S'
#define PROT_PING_LENGTH 4
#define PROT_LOBBY_ID 'T'
#define PROT_LOBBY_LENGTH 4
#define PROT_READY_PUSH_ID 'U'
#define PROT_READY_PUSH_LENGTH 4
#define PROT_HEADING_ID 'V'
#define PROT_HEADING_LENGTH 28

// -=- ACTOR SPECIFIC DATA -=-
#define ACTOR_TYPE_NONE		0x0
#define ACTOR_TYPE_PERSON	0x1
#define ACTOR_TYPE_TARGET	0x2
#define ACTOR_TYPE_OBSTACLE	0x4

// +++ PLAYER +++
#define PLAYER_TYPE_HUMAN	0x1
#define PLAYER_TYPE_LAN		0x2
#define PLAYER_TYPE_AI		0x4

#define PLAYER_NAME_COLOR_R		HUD_COLOR_R
#define PLAYER_NAME_COLOR_G		HUD_COLOR_G
#define PLAYER_NAME_COLOR_B		HUD_COLOR_B

#define DECELERATION_FACTOR		5.0f
#define PLAYER_FLYING_TRESHOLD	0.3f	// time in seconds, before a player counts as "flying"

#define MAX_NAME_LENGTH		30

// -+- AI Player -+-
#define AI_MAX_ROTATION		 45.0f
#define AI_ROTATION_ANGLE	 10.0f
#define AI_GAME_TYPE_FREE	 0x1
#define AI_GAME_TYPE_DOWN	 0x2
//#define AI_MAX_START_X		 10.0f
//#define AI_MIN_START_X		-10.0f
//#define AI_MAX_START_Z		 10.0f
//#define AI_MIN_START_Z		-10.0f
#define NUM_AI_PLAYERS		 5

// +++ TARGETS +++
// -+- SlalomGate -+-
#define SLALOM_GATE_WIDTH	2.0f
#define SLALOM_GATE_HEIGHT	2.5f
#define SLALOM_ROUTE_LENGTH		5
#define SLALOM_GATE_DISTANCE	15.0f
#define SLALOM_GATE_VARIATION	5.0f
#define SLALOM_GATE_TIMEOUT_VALUE	25.0f
#define SLALOM_GATE_MISS_PENALTY	10.0f
#define SLALOM_GATE_IS_ACTIVE	0x0
#define SLALOM_GATE_DEACTIVATED	0x1
#define SLALOM_GATE_TIMED_OUT	0x2

// +++ OBSTACLES +++
#define OBSTACLE_DIFFERENT_TYPES	2
#define OBSTACLE_NUM		100
#define OBSTACLE_MIN_X		-680.0f
#define OBSTACLE_MAX_X		 650.0f
#define OBSTACLE_MIN_Z		-620.0f
#define OBSTACLE_MAX_Z		 680.0f
#define OBSTACLE_VOID_MIN_X	-10.0f
#define OBSTACLE_VOID_MAX_X	 10.0f
#define OBSTACLE_VOID_MIN_Z	-10.0f
#define OBSTACLE_VOID_MAX_Z	 10.0f

// -=- AVATAR SPECIFIC DATA -=-
#define AVATAR_TYPE_BOARDER		0x0
#define AVATAR_TYPE_CARVER		0x1
#define AVATAR_SIZE				1.8f	// meter
#define AVATAR_MAX_NUM_SKIN_COL	4
#define AVATAR_MAX_NUM_HAIR_COL	3

#define AVATAR_COLOR_HAIR		0x1
#define AVATAR_COLOR_SKIN		0x2
#define AVATAR_COLOR_SHIRT		0x4
#define AVATAR_COLOR_GLOVES		0x10
#define AVATAR_COLOR_TROUSERS	0x20
#define AVATAR_COLOR_BOOTS		0x40

#define _BOARDER_BOARD_COLOR	Color(0.0f,0.13f, 0.5f)

// -=- MATTERHORN DATA -=-
#define M_LENGTH 80
#define QUADS_NUM M_LENGTH*M_LENGTH

#define MATTERHORN_SNOW_BORDER		3600.0f
#define MATTERHORN_ROCK_BORDER		3300.0f
#define MATTERHORN_GRAVITY			9.81f
#define MATTERHORN_MAX_SPEED		55.5f
#define MATTERHORN_MAX_JUMP_BONUS	-15.0f
#define MONSTER_JUMP_PERCENT		0.95f
#define MONSTER_JUMP_POINTS			100
#define TRACK_BORDER				-850.0f

// -=- PARTICLE SYSTEM -=-
#define MIN_TIMEDIFF_TO_UPDATE	0.04f
#define MAX_RUNS_TILL_UPDATE	3

#define SPEED_FAKTOR_RAINDROP	Vector3D(4.0f, 4.0f, 4.0f)
#define SPEED_FAKTOR_SNOWFLAKE	Vector3D(2.0f, 2.0f, 2.0f)
#define VARIANZ_RAINDROP		Vector3D(0.1f, 0.0f, 0.1f)
#define VARIANZ_SNOWFLAKE		Vector3D(0.2f, 0.1f, 0.2f)
#define TIME_TILL_CHANGE_VARIANZ	0.8f

// -=- MENUS -=-
#define MENU_COORD_X	-1.4f
#define MENU_COORD_Y	 0.9f
#define MENU_COORD_Z	-3.0f
#define MENU_DISTANCE	 0.17f
#define MENU_ROTATION_RATE	15.0f

#ifdef NIGHTLY_BUILD
#define MENU_TITLE_MAIN			"WinterGames - Nightly Build!"
#else
#define MENU_TITLE_MAIN			"OpenGL WinterGames 2005"
#endif
#define MENU_TITLE_NEWGAME		"Neues Spiel"
#define MENU_TITLE_LANGAME		"Netzwerkspiel"
#define MENU_TITLE_LAN_CREATE	"Erstellen"
#define MENU_TITLE_LAN_JOIN		"Teilnehmen"
#define MENU_TITLE_LAN_LOBBY	"Lobby"
#define MENU_TITLE_OPTIONS		"Optionen"
#define MENU_TITLE_HIGHSCORE	"Highscore"
#define MENU_TITLE_CLEAR_HS		"Highscore leeren"
#define MENU_TITLE_OPT_PLAYER	"Spieler"
#define MENU_TITLE_OPT_VIDEO	"Video"
#define MENU_TITLE_CREDITS		"Credits"
#define MENU_TITLE_EXIT			"Sicher?"
#define MENU_CURSOR_CHAR		'_'

// -=- DEBUG-Ausgaben -=-
#ifdef _DEBUG
//#define _DEBUG_MAIN
//#define _DEBUG_RANGER
//#define _DEBUG_PLAYER
//#define _DEBUG_MENU
//#define _DEBUG_EMITTER
#define _DEBUG_NETWORK
//#define _DEBUG_NETWORK_TRAFFIC
#endif

#endif // define_h_

