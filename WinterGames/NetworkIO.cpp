#include "networkio.h"

NetworkIO::NetworkIO(MountainRanger *newRanger)
{
	mRanger = newRanger;
	helper = new PlayerPacketHelper();
	TotalSockets = 0;

	networkIsWorking = false;
	serverIsWorking = false;
	clientIsWorking = false;

	// setting Time to wait in select()
	tv.tv_sec = 0;	// second
	tv.tv_usec = 1;	// 1/1000 second (microsecond)
	tvp = &tv;		// sry, but this has to be done!
}

NetworkIO::~NetworkIO(void)
{
	delete helper;
}

void NetworkIO::update()
{
	// in case ranger doesnt request network->exit right away
	if (!mRanger->runServer() && !mRanger->runClient()) {
		closeAllSockets();
		return;
	}

	// if in reality server or client is running->updateCheck()
	if (serverIsRunning() || clientIsRunning()) {
		networkCheck();
	}

	// if any data is available: send it to ranger
	if (TotalSockets > 0) {
        for (unsigned int i = 0; i < playersTraffic.size(); i++) {
            if (playersTraffic[i]->packetsReady()) {
                mRanger->processNetworkPacket(playersTraffic[i]->getPacket(), i);
                i--;
			}
		}
	}

	while (mRanger->socketToClose()) {
		FreeSocketInformation((unsigned long)mRanger->getSocketToClose());
	}
	
	// if ranger requests server or client start each and reInit network if needed
	if (mRanger->runServer()) startServer();

	if (mRanger->runClient()) {
		// TODO: auf std::string umstellen oder anders optimieren
		char* buffer = new char[mRanger->getIPAddress().length() + 1];
		for(unsigned int i = 0; i < mRanger->getIPAddress().length(); i++)
			buffer[i] = mRanger->getIPAddress()[i];
		buffer[mRanger->getIPAddress().length()] = '\0';

		startClient(buffer);
	
		delete [] buffer;
	}
}

void NetworkIO::startNetwork()
{
	if (networkIsRunning()) return;
	networkIsWorking = initNetwork();
#ifdef _DEBUG
	std::cout << "Network ";
	if (networkIsRunning()) std::cout << "successfully ";
	else std::cout << "not ";
	std::cout << "started!\n";
#endif
}

void NetworkIO::startServer()
{
	startNetwork();
	if (!networkIsRunning()) return;
	if (serverIsRunning()) return;
	serverIsWorking = initServer();
#ifdef _DEBUG
	std::cout << "Server ";
	if (serverIsRunning()) std::cout << "successfully ";
	else std::cout << "not ";
	std::cout << "started!\n";
#endif
}

void NetworkIO::startClient(char* ipAdr)
{
	startNetwork();
	if (!networkIsRunning()) return;
	if (clientIsRunning()) return;
	clientIsWorking = initClient(ipAdr);
#ifdef _DEBUG
	std::cout << "Client ";
	if (clientIsRunning()) std::cout << "successfully ";
	else std::cout << "not ";
	std::cout << "started!\n";
#endif
	if (!clientIsRunning()) mRanger->networkError(CLIENT__SERVER_NOT_FOUND);
}

bool NetworkIO::networkIsRunning()
{
	return networkIsWorking;
}

bool NetworkIO::serverIsRunning()
{
	return serverIsWorking;
}

bool NetworkIO::clientIsRunning()
{
	return clientIsWorking;
}

void NetworkIO::closeAllSockets()
{
	while (TotalSockets > 0) {
		FreeSocketInformation(0);
	}
	clientIsWorking = false;
}

bool NetworkIO::initNetwork() {
	if ((WSAStartup(MAKEWORD(2,0),&wsaData)) != 0) {
#ifdef _DEBUG
		std::cout << "Startup failed\n";
#endif
		return false;
	}

	// create socket
	if ((ListenSocket = WSASocketW(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET) {
#ifdef _DEBUG
		std::cout << "Winsock error: WSASocket() failed " << WSAGetLastError() << std::endl;
#endif
        return false;
	}

	return true;
}

bool NetworkIO::initServer() {
	// binding to socket

    InternetAddr.sin_family = AF_INET;
    InternetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    InternetAddr.sin_port = htons(PORT);

	if (bind(ListenSocket, (SOCKADDR *) &InternetAddr, sizeof(InternetAddr)) == SOCKET_ERROR) {
#ifdef _DEBUG
		std::cout << "Winsock error: Binding failed " << WSAGetLastError() << std::endl;
#endif
        return false;
	}

	if (listen(ListenSocket, 5)) {
#ifdef _DEBUG
		std::cout << "Winsock error: listen failed " << WSAGetLastError() << std::endl;
#endif
        return false;
	}

    // Change the socket mode on the listening socket from blocking to non-block 
    NonBlock = 1;
	if (ioctlsocket(ListenSocket, FIONBIO, &NonBlock) == SOCKET_ERROR) {
#ifdef _DEBUG
		std::cout << "ioctlsocket() failed\n";
#endif
        return false;
	}

	return true;
}

bool NetworkIO::initClient(char* ipAdr) {
    struct sockaddr_in addr;
    memset((char *) &addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ipAdr);
    addr.sin_port = htons(PORT);

    SOCKET fd = socket(AF_INET, SOCK_STREAM, 0);		// create socket
    if (fd < 0) {
#ifdef _DEBUG
		std::cout << "creating socket error client" << std::endl;
#endif
		return false;
	}

    if (connect(fd, (struct sockaddr*) &addr, sizeof(addr))) {
#ifdef _DEBUG
		std::cout << "connecting socket error client" << std::endl;
#endif
		return false;
	}

#ifdef _DEBUG
	std::cout << "socket client side connected" << std::endl;
#endif
	return CreateSocketInformation(fd);
}

bool NetworkIO::networkCheck()
{
    // Initialize the Read and Write socket set.
    FD_ZERO(&Reader);
    FD_ZERO(&Writer);

    // Check for connection attempts.
    FD_SET(ListenSocket, &Reader);

	for (unsigned int i = 0; i < TotalSockets; i++) {
		if (mRanger->packetsReady(i)) FD_SET(SocketList[i]->Socket, &Writer);
		FD_SET(SocketList[i]->Socket, &Reader);
	}

	if ((Total = select(0, &Reader, &Writer, NULL, tvp)) == SOCKET_ERROR) {
#ifdef _DEBUG
		std::cout << "Winsock error: select function returned with error " << WSAGetLastError() << std::endl;
#endif
        return false;
	}

    // Check for arriving connections on the listening socket.
	if (FD_ISSET(ListenSocket, &Reader)) {
        Total--;
		if ((AcceptSocket = accept(ListenSocket, NULL, NULL)) != INVALID_SOCKET) {
            // Set the accepted socket to non-blocking mode so the server will
            // not get caught in a blocked condition on WSASends
            NonBlock = 1;
			if (ioctlsocket(AcceptSocket, FIONBIO, &NonBlock) == SOCKET_ERROR) {
#ifdef _DEBUG
				std::cout << "Winsock error: ioctlsocket() failed with error " << WSAGetLastError() << std::endl;
#endif
                return false;
			}
			if (CreateSocketInformation(AcceptSocket) == false) {
                return false;
			}
		} else {
			if (WSAGetLastError() != WSAEWOULDBLOCK) {
#ifdef _DEBUG
				std::cout << "accept() failed with error " << WSAGetLastError() << std::endl;
#endif
                return false;
			}
		}
	}

    // Check each socket for Read and Write notification for Total number of sockets
	for (unsigned int i = 0; Total > 0 && i < TotalSockets; i++) {
        LPSOCKET_INFORMATION SocketInfo = SocketList[i];
        
		// If the Reader is marked for this socket then this means data
        // is available to be read on the socket.
		if (FD_ISSET(SocketInfo->Socket, &Reader)) {
            Total--;
            
            SocketInfo->DataBuf.buf = SocketInfo->Buffer;
            SocketInfo->DataBuf.len = BUFFERSIZE;

            Flags = 0;
			if (WSARecv(SocketInfo->Socket, &(SocketInfo->DataBuf), 1, &SocketInfo->RecvBytes, &Flags, NULL, NULL) == SOCKET_ERROR) {
				if (WSAGetLastError() != WSAEWOULDBLOCK) {
#ifdef _DEBUG
					std::cout << "Winsock: Receive failed with error\n";
#endif
                    FreeSocketInformation(i);
				}
                continue;
			} else {
#ifdef SHOW_IO_MSGS
				std::cout << "recevied: " << SocketInfo->RecvBytes << " (Socket: " << i << ")\n";
#endif
				playersTraffic[i]->addBuffer(SocketInfo->Buffer, SocketInfo->RecvBytes);
                // If zero bytes are received, this indicates connection is closed.
				if (SocketInfo->RecvBytes == 0) {
                    FreeSocketInformation(i);
                    continue;
				}
				SocketInfo->RecvBytes = 0;
				//TODO: repair that!
				//int k = 0;
				//while (SI->Buffer[k] != '\0') {
    //                SI->Buffer[k] = '\0';
				//	k++;
				//}
			}
		}

        // If the Writer is marked on this socket then this means the internal
        // data buffers are available for more data.
		if (FD_ISSET(SocketInfo->Socket, &Writer)) {
            Total--;

			unsigned int length = 0;
			std::vector<char> temp;
			while (mRanger->packetsReady(i)) {
				char *current = mRanger->getPacket(i);
				unsigned int length = helper->getLength(current[0]);
				for (unsigned int j = 0; j < length + 1; j++) {
					temp.push_back(current[j]);
				}
				delete [] current;
			}
			char *sendBuffer = new char[temp.size()];
#ifdef SHOW_IO_MSGS
			std::cout << "sent: " << (unsigned int)temp.size() << " (Socket: " << i << ")\n";
#endif
			for (unsigned int j = 0; j < temp.size(); j++) {
				sendBuffer[j] = temp[j];
			}
			SocketInfo->DataBuf.buf = sendBuffer;
			SocketInfo->DataBuf.len = (unsigned long)temp.size();

			if (WSASend(SocketInfo->Socket, &(SocketInfo->DataBuf), 1, &SocketInfo->SendBytes, 0, NULL, NULL) == SOCKET_ERROR) {
				if (WSAGetLastError() != WSAEWOULDBLOCK) {
#ifdef _DEBUG
					std::cout << "Send failed with error\n";
#endif
                    FreeSocketInformation(i);
					//delete[] sendBuffer;
				}
                continue;
			} else {
                SocketInfo->SendBytes = 0;
			}
			delete[] sendBuffer;
		}
	}

	return true;
}

bool NetworkIO::CreateSocketInformation(SOCKET s)
{
	if (!mRanger->createPlayer()) return false;
#ifdef _DEBUG
	std::cout << "Accepted socket\n";
#endif

    if ((SI = (LPSOCKET_INFORMATION) GlobalAlloc(GPTR, sizeof(SOCKET_INFORMATION))) == NULL) {
#ifdef _DEBUG
		std::cout << "Winsock error: GlobalAlloc() failed\n";
#endif
        return false;
	}

    // Prepare SocketInfo structure for use.
    SI->Socket = s;
    SI->SendBytes = 0;
    SI->RecvBytes = 0;

    SocketList[TotalSockets] = SI;
    TotalSockets++;

	playersTraffic.push_back(new PlayerPacketReceive());
    return true;
}

void NetworkIO::FreeSocketInformation(DWORD Index)
{
    LPSOCKET_INFORMATION SI = SocketList[Index];

    closesocket(SI->Socket);

#ifdef _DEBUG
	std::cout << "Closing socket\n";
#endif

    GlobalFree(SI);

	PlayerPacketReceive *oldPlayerPacket = playersTraffic[Index];
    // Remove from the socket array
    for (DWORD i = Index; i < TotalSockets; i++) {
        SocketList[i] = SocketList[i + 1];
		playersTraffic[i] = playersTraffic[i + 1];
	}
	playersTraffic.pop_back();
	delete oldPlayerPacket;
	mRanger->killPlayer(Index);

    TotalSockets--;
}
