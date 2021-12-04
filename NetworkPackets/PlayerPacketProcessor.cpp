#include ".\playerpacketprocessor.h"

PlayerPacketProcessor::PlayerPacketProcessor(void)
{
	whoAmI = 0;
	timeOut = false;
}

PlayerPacketProcessor::~PlayerPacketProcessor(void)
{
}

void PlayerPacketProcessor::update(float timeDiff, bool server)
{
	for (unsigned int i = 0; i < socketPingTime.size(); i++) {
		socketPingTime[i] += timeDiff;
		// search if socket is already in delete list
		bool found = false;
		for (unsigned int j = 0; j < socketsToLateDelete.size(); j++) {
			if (socketsToLateDelete[j] == serverPlayerNum(socketMapper[i])) found = true;
		}
		if (found) continue;

		// if time is up, send msg and send to socketLateDeleteList
		if (socketPingTime[i] > NETWORK_PING_RECV_TIME) {
			socketsToLateDelete.push_back(serverPlayerNum(socketMapper[i]));
			ProtError *protError = new ProtError();
			protError->giveInformation(0, SERVER_CLIENT__TIME_OUT);
			sendPacket(serverPlayerNum(socketMapper[i]), protError);
			if (!server) timeOut = true;
#ifdef _DEBUG
			std::clog << "Connection timed out! Socket: " << i << " // PlayerNum: " << socketMapper[i] << std::endl;
#endif
		}
	}

	// adding socket to delete, if no packet to send anymore
	for (unsigned int i = 0; i < socketsToLateDelete.size(); i++) {
		if (!packetsReady(getSocketForPlayer(socketsToLateDelete[i]))) {
			addSocketToDelete(socketsToLateDelete[i]);
			socketsToLateDelete[i] = socketsToLateDelete[socketsToLateDelete.size()-1];
			socketsToLateDelete.pop_back();
			i--;
		}
	}
}

void PlayerPacketProcessor::addPlayerSocket(unsigned int playerNum)
{
	socketMapper.push_back(playerNum);
	socketPingTime.push_back(0.0f);
	std::vector<NetworkPacket*> newPacketArray;
	sendPacketList.push_back(newPacketArray);
	timeOut = false;
}

unsigned int PlayerPacketProcessor::deleteSocket(unsigned int socketNum)
{
	// get playernumber of deleted player
	unsigned int playerNum = socketMapper[socketNum];
	// delete all char* from packets
	for (unsigned int i = 0; i < sendPacketList[socketNum].size(); i++) {
		delete sendPacketList[socketNum][i];
	}
	// drop player and close hole, also for outgoing pakets
	for (unsigned int i = socketNum; i < socketMapper.size() - 1; i++) {
		socketMapper[i] = socketMapper[i+1];
		socketPingTime[i] = socketPingTime[i+1];
		sendPacketList[i] = sendPacketList[i+1];
	}
	socketMapper.pop_back();
	socketPingTime.pop_back();
	sendPacketList.pop_back();
	return playerNum;
}

void PlayerPacketProcessor::addSocketToDelete(unsigned int playerNum)
{
	playerNum = serverPlayerNum(playerNum);
	if (!playerExists(playerNum)) return;
	socketsToKill.push_back(getSocketForPlayer(playerNum));
}

void PlayerPacketProcessor::killAllSockets()
{
	for (unsigned int i = 0; i < socketMapper.size(); i++) {
		socketsToKill.push_back(0);
	}
}

unsigned int PlayerPacketProcessor::getSocketToDelete()
{
	if (!socketToDelete()) return 0;
	unsigned int toKill = socketsToKill[0];
	socketsToKill.erase(socketsToKill.begin());
	return toKill;
}

bool PlayerPacketProcessor::socketToDelete()
{
	if (socketsToKill.size() > 0) return true;
	return false;
}

void PlayerPacketProcessor::changePlayerNum(unsigned int oldPlayerNum, unsigned int newPlayerNum)
{
	if (!playerExists(oldPlayerNum)) return;
	unsigned int socket = getSocketForPlayer(oldPlayerNum);
	socketMapper[socket] = newPlayerNum;
}

bool PlayerPacketProcessor::playerExists(unsigned int playerNum)
{
	for (unsigned int i = 0; i < socketMapper.size(); i++) {
		if (socketMapper[i] == playerNum) return true;
	}
	return false;
}

char* PlayerPacketProcessor::getPacket(unsigned int socketNum)
{
	if (!packetsReady(socketNum)) return NULL;

	NetworkPacket *packetToSend = sendPacketList[socketNum][0];
	for (unsigned int i = 0; i < sendPacketList[socketNum].size()-1; i++) {
		sendPacketList[socketNum][i] = sendPacketList[socketNum][i+1];
	}
	sendPacketList[socketNum].pop_back();
	char* sendString = packetToSend->getString();
	delete packetToSend;
	return sendString;
}

bool PlayerPacketProcessor::packetsReady(unsigned int socketNum)
{
	if (socketNum >= sendPacketList.size()) return false;
	if (sendPacketList[socketNum].size() > 0) return true;
	return false;
}

bool PlayerPacketProcessor::connectionsOpen()
{
	if (socketMapper.size() > 0) return true;
	return false;
}

void PlayerPacketProcessor::sendPacket(unsigned int playerNumToSend, NetworkPacket *packet)
{
	playerNumToSend = serverPlayerNum(playerNumToSend);
	if (sendToAll(playerNumToSend)) {
		for (unsigned int i = 0; i < socketMapper.size(); i++) {
			ProtCopy *copy = new ProtCopy(packet);
			sendPacketList[i].push_back(copy);
		}
		delete packet;
	} else {
		if (!playerExists(playerNumToSend)) return;
        unsigned int socketNum = getSocketForPlayer(playerNumToSend);
        sendPacketList[socketNum].push_back(packet);
	}
}

unsigned int PlayerPacketProcessor::getSocketForPlayer(unsigned int playerNum)
{
	for (unsigned int i = 0; i < socketMapper.size(); i++) {
		if (socketMapper[i] == playerNum) return i;
	}
	return 0;
}

bool PlayerPacketProcessor::sendToAll(unsigned int playerNum)
{
	if (playerNum == MAX_PLAYERS) return true;
	return false;
}

unsigned int PlayerPacketProcessor::serverPlayerNum(unsigned int playerNum)
{
	if (playerNum == 0) return whoAmI;
	if (playerNum == whoAmI) return 0;
	return playerNum;
}

void PlayerPacketProcessor::setWhoAmI(unsigned int newWhoAmI)
{
	whoAmI = newWhoAmI;
}

void PlayerPacketProcessor::pingRecv(unsigned int playerNum)
{
	if (playerExists(playerNum))
        socketPingTime[getSocketForPlayer(playerNum)] = 0.0f;
}

void PlayerPacketProcessor::addSocketToLateDelete(unsigned int socketNum)
{
	socketsToLateDelete.push_back(serverPlayerNum(socketMapper[socketNum]));
}

bool PlayerPacketProcessor::gotTimeOut()
{
	return timeOut;
}