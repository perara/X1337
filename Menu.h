#pragma once
#include "Scene.h"
#include "Globals.h"

#include <SFML\Graphics.hpp>
#include <map>
#include <list>

class Menu: public Scene{
public:

	Menu(sf::RenderWindow& window);

	virtual void draw();
	virtual void process();
	virtual void init();
	virtual void input(sf::Event&);
	virtual void reset();

private:
	// Loaders
	void loadMenuOptions();


	// Draw Functions
	void drawStageSelect();
	void drawMainMenu();
	void drawOptions(Globals::State);
	void drawGameTitle();


	const enum Options
	{
		NEW_GAME,
		LOAD_GAME,
		CREDITS,
		EXIT_GAME,
		BACK,
		MENU_OPT_COUNT
	};

	int currentOption;
	int getCurrentOption();
	void setCurrentOption(int);


	// Option lists
	std::map<Globals::State, std::map<Menu::Options, std::string>> optMap;

	std::map<Globals::State, std::map<Menu::Options, sf::Text>> option;

};