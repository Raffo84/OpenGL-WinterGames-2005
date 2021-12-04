#pragma once

#ifdef _DEBUG
//#define SHOW_IO_MSGS
#endif

#include <iostream>
#include <vector>
#include <winsock2.h>

#include "../mountainLib/MountainRanger.h"
#include "../NetworkPackets/PlayerPacketReceive.h"
#include "../NetworkPackets/PlayerPacketHelper.h"
#include "define.h"

class NetworkIO
{
public:
	NetworkIO(MountainRanger *newRanger);
	~NetworkIO(void);

	void update();

	void startNetwork();
	void startServer();
	void startClient(char* ipAdr);
	bool networkCheck();

	bool networkIsRunning();
	bool serverIsRunning();
	bool clientIsRunning();

	void closeAllSockets();

protected:
	MountainRanger *mRanger;
	PlayerPacketHelper *helper;

	// array for uncompleted packets
	std::vector<PlayerPacketReceive*> playersTraffic;

	typedef struct _MYSOCKET_INFORMATION {
		CHAR Buffer[BUFFERSIZE];
		WSABUF DataBuf;
		SOCKET Socket;
		DWORD SendBytes;
		DWORD RecvBytes;
	} SOCKET_INFORMATION, * LPSOCKET_INFORMATION;

	struct timeval tv;
	struct timeval *tvp;

	bool initNetwork();
	bool initServer();
	bool initClient(char* ipAdr);

	bool networkIsWorking;
	bool serverIsWorking;
	bool clientIsWorking;	
	
	bool CreateSocketInformation(SOCKET s);
	void FreeSocketInformation(DWORD Index);

	DWORD TotalSockets;
	LPSOCKET_INFORMATION SocketList[FD_SETSIZE];

	SOCKET ListenSocket;
	SOCKET AcceptSocket;
	SOCKADDR_IN InternetAddr;
	WSADATA wsaData;
	FD_SET Writer;
	FD_SET Reader;
	ULONG NonBlock;
	DWORD Flags;
	int Total;
	LPSOCKET_INFORMATION SI;
};
