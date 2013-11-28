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

	void drawPause(int xOffSet, int yOffset); // Should be accessible to gameEngine 
	int getStageSelectOption();
private:
	// Loaders
	void loadMenuOptions();


	// Draw Functions
	void drawStageSelect();
	void drawMainMenu();
	void drawOptions(Globals::State, int xOffset = 0, int yOffset = 0, sf::Color color = sf::Color(139,137,137));
	void drawGameTitle();

	// Input functions
	void stageSelectInput(sf::Event&);


	const enum Options
	{
		// Main menu opts
		NEW_GAME,
		LOAD_GAME,
		CREDITS,
		EXIT_GAME,	

		// Stage Select opts
		SELECT_STAGE,
		BACK,

		// In game pause opts
		CONTINUE_GAME,
		TO_MAIN_MENU,
		MENU_OPT_COUNT
	};

	int currentOption; // Current selected menu option
	int getCurrentOption();
	void setCurrentOption(int);

	int numStages; // This describes number of stages
	int stageSelectOption; // Current selected stage option
	void setStageSelectOption(int);

	// Option lists
	std::map<Globals::State, std::map<Menu::Options, std::string>> optMap;
	std::map<Globals::State, std::map<Menu::Options, sf::Text>> option;

};