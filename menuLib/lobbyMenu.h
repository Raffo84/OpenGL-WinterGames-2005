#pragma once
#include "menu.h"
#include "../mountainLib/Player.h"
#include "../WinterGames/define.h"

class lobbyMenu :
	public menu
{
public:
	lobbyMenu(void);
	lobbyMenu(menu* parent);
	~lobbyMenu(void);

	void setPlayerList(std::vector<Player*>* newList) { playerList = newList; }
	int  getCurrentPlayer() const;
	void setLobbyType(bool isClient = true);
	bool isClientLobby() const { return isClient; }
	void setGameType(int);
	void setIpList(std::vector<std::string>* newList);

	//void setReadyFlags(int newBitmask) { playerReadyFlags = newBitmask; }
	//void setReadyFlags(int playerIndex, bool isReady);
	//int  getReadyFlags() const { return playerReadyFlags; }

	void changeSelection(int offset);
	void render() const;
	void show();
	void reset();

protected:
	//std::vector<const Player*>	myPlayerList;
	std::vector<Player*>*		playerList;
	std::vector<std::string>	ipAddressList;
	bool	isClient;
	int		gameFlags;
	//int		playerReadyFlags;

	void init();
	void dispatchCallback(unsigned int index) const;
};
