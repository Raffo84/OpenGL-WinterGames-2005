#pragma once

#include <iostream>
#include <vector>
#include "../WinterGames/define.h"

#include "NetworkPacket.h"
#include "ProtSpeedAngle.h"
#include "ProtPosition.h"
#include "ProtCrash.h"
#include "ProtStart.h"
#include "ProtFinish.h"
#include "ProtInitInformation.h"
#include "ProtPlayerNum.h"
#include "ProtCopy.h"
#include "ProtError.h"

class PlayerPacketProcessor
{
public:
	PlayerPacketProcessor(void);
	~PlayerPacketProcessor(void);

	void update(float timeDiff, bool server);

	void addPlayerSocket(unsigned int playerNum);
	unsigned int deleteSocket(unsigned int socketNum);
	void addSocketToDelete(unsigned int playerNum);
	void killAllSockets();
	unsigned int getSocketToDelete();
	bool socketToDelete();
	void changePlayerNum(unsigned int oldPlayerNum, unsigned int newPlayerNum);
	bool playerExists(unsigned int playerNum);
	char* getPacket(unsigned int socketNum);
	bool packetsReady(unsigned int socketNum);
	unsigned int serverPlayerNum(unsigned int playerNum);
	bool connectionsOpen();
	void setWhoAmI(unsigned int newWhoAmI);
	void pingRecv(unsigned int playerNum);
	void addSocketToLateDelete(unsigned int socketNum);
	bool gotTimeOut();

	// packet sending
	void sendPacket(unsigned int playerNum, NetworkPacket *packet);

protected:
	std::vector<unsigned int>					socketMapper;
	std::vector<float>							socketPingTime;
	std::vector< std::vector<NetworkPacket*> >	sendPacketList;
	std::vector<unsigned int>					socketsToKill;
	std::vector<unsigned int>					socketsToLateDelete;
	unsigned int whoAmI;
	bool timeOut;

	unsigned int getSocketForPlayer(unsigned int playerNum);
	bool sendToAll(unsigned int playerNum);
	
};
