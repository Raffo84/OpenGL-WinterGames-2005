#pragma once

#include <vector>
#include "../coolLib/GL/glut.h"
#include "../coolLib/Color.h"
#include "../WinterGames/define.h"

class menu;
class credMenu;
class lobbyMenu;
class menuEntry;
#include "menuEntry.h"

class menu
{
public:
	menu(void);
	menu(menu* parent);
	~menu(void);

	virtual void addEntry(const std::string label);
	virtual void addSubMenu(const std::string label, menu* submenu);
	virtual void render() const;
	virtual void show();
	virtual void hide();
	int  displayMessage(std::string msgText);
	bool isVisible() const;
	void callbackFunc(void (*func)(menu*, unsigned int));
	void  setParent(menu* newParent);
	menu* getParent() { return myParent; }
	menu* getCurrentMenu();
	menuEntry* getCurrentEntry() { return myEntries[activeIndex]; }
	menuEntry* getEntry(int index);
	void setTitle(const std::string newTitle) { myTitle = newTitle; }
	std::string getTitle() const { return myTitle; }
	virtual void setColors(Color font, Color selection, Color title);
	virtual void changeSelection(int offset);
	virtual void fire() const;
	virtual void reset();

	virtual void operator=(const menu* rhs);

	static const int MSG_NO_ERROR		= 0x0;
	static const int MSG_OTHER_ERROR	= 0x1;

protected:
	std::vector<menu*>		mySubmenus;
	std::vector<menuEntry*>	myEntries;
	std::string	myTitle;
	std::string msgText;
	menu*		myParent;
	Color		fontColor, activeColor, titleColor;
	int			activeIndex;
	bool		isVis;

	void (*myCallbackFunc)(menu*, unsigned int);

	virtual void dispatchCallback(unsigned int index) const;
};
