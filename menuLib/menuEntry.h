#pragma once

#include "menu.h"
#include "../WinterGames/define.h"

class menuEntry
{
	friend class menu;
	friend class credMenu;
	friend class lobbyMenu;
public:
	menuEntry(std::string, unsigned int idx, menu*);
	menuEntry(std::string, unsigned int idx, menu*, unsigned int subMenuIndex);
	~menuEntry(void);

	std::string getLabel() const { return label; }
	void setLabel(std::string lbl);
	bool isSubmenu() const { return isSub; }
	unsigned int getSubmenuIndex() const { return subIndex; }

protected:
	std::string		label;
	menu*			parentMenu;
	bool			isSub;
	unsigned int	index;
	unsigned int	subIndex;

	void render(unsigned int position) const;
};
