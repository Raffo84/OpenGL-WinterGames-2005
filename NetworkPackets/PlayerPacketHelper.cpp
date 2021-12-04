#include ".\playerpackethelper.h"

PlayerPacketHelper::PlayerPacketHelper(void)
{
}

PlayerPacketHelper::~PlayerPacketHelper(void)
{
}

unsigned int PlayerPacketHelper::getLength(char prot)
{
	unsigned int length = 0;
	if (prot == PROT_SPEED_ANGLE_ID) length = PROT_SPEED_ANGLE_LENGTH;
	if (prot == PROT_POSITION_ID) length = PROT_POSITION_LENGTH;
	if (prot == PROT_CRASH_ID) length = PROT_CRASH_LENGTH;
	if (prot == PROT_START_ID) length = PROT_START_LENGTH;
	if (prot == PROT_FINISH_ID) length = PROT_FINISH_LENGTH;
	if (prot == PROT_INIT_INFORMATION_ID) length = PROT_INIT_INFORMATION_LENGTH;
	if (prot == PROT_PLAYER_NUM_ID) length = PROT_PLAYER_NUM_LENGTH;
	if (prot == PROT_CREATE_PLAYERS_ID) length = PROT_CREATE_PLAYERS_LENGTH;
	if (prot == PROT_DELETE_PLAYERS_ID) length = PROT_DELETE_PLAYERS_LENGTH;
	if (prot == PROT_GAME_TYPE_ID) length = PROT_GAME_TYPE_LENGTH;
	if (prot == PROT_STYLE_POINTS_ID) length = PROT_STYLE_POINTS_LENGTH;
	if (prot == PROT_FINAL_TIME_ID) length = PROT_FINAL_TIME_LENGTH;
	if (prot == PROT_LAST_FALL_SPEED_ID) length = PROT_LAST_FALL_SPEED_LENGTH;
	if (prot == PROT_DAY_TIME_ID) length = PROT_DAY_TIME_LENGTH;
	if (prot == PROT_READY_ID) length = PROT_READY_LENGTH;
	if (prot == PROT_ERROR_ID) length = PROT_ERROR_LENGTH;
	if (prot == PROT_VERSION_ID) length = PROT_VERSION_LENGTH;
	if (prot == PROT_PING_ID) length = PROT_PING_LENGTH;
	if (prot == PROT_LOBBY_ID) length = PROT_LOBBY_LENGTH;
	if (prot == PROT_READY_PUSH_ID) length = PROT_READY_PUSH_LENGTH;
	if (prot == PROT_HEADING_ID) length = PROT_HEADING_LENGTH;
	return length;
}