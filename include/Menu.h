#pragma once
#include "Scene.h"
#include "GameEngine.h"
#include "ResourceHandler.h"

#include <SFML\Graphics.hpp>
#include <map>
#include <list>

/// <summary>
/// A class which is called by GameEngine. This class serves the menu for the whole application. It is based on GameState
/// </summary>
class Menu : public Scene{
public:

	Menu(sf::RenderWindow& window, GameState& state, std::shared_ptr<ResourceHandler>& resourceHandler);

	// Scene virtuals
	virtual void draw();
	virtual void process();
	virtual void input(sf::Event&);



	void init();
	void reset();
	bool getHardmodeSelected();
	void resetCurrentOption();
	void drawPause(int xOffSet, int yOffset); // Should be accessible to gameEngine 
	int getStageSelectOption();
	void setStageSelectOption(int);
	void setMessageOfTheDayId(int);
private:
	// Loaders
	void loadMenuOptions();


	// Draw Functions
	void drawStageSelect();
	void drawMainMenu();
	void drawOptions(GameState, int xOffset = 0, int yOffset = 0, sf::Color color = sf::Color(139, 137, 137));
	void drawGameTitle();
	void drawHighScore();
	void drawCredits();

	// Input functions
	void stageSelectInput(sf::Event&);


	const enum Options
	{
		// Main menu opts
		NEW_GAME,
		HIGHSCORE,
		CREDITS,
		EXIT_GAME,

		LOAD_GAME,

		// Stage Select opts
		STAGE_SELECT,
		BACK,

		// Difficulty Select opts
		NORMAL,
		HARD,

		// In-game pause opts
		CONTINUE_GAME,
		TO_MAIN_MENU,

		// In-game gameover opts
		RESTART_STAGE,
		TO_MAIN_MENU2,

		// Win game Options
		NEXT_STAGE,
		RESTART_STAGE_2,
		TO_MAIN_MENU_3,

		MENU_OPT_COUNT
	};

	int currentOption; // Current selected menu option
	int getCurrentOption();
	void setCurrentOption(int);

	int numStages; // This describes number of stages
	int stageSelectOption; // Current selected stage option
	int messageOfTheDayId; //Current message of the day id (From ResourceHandler)

	// Option lists
	std::map<GameState, std::map<Menu::Options, std::string>> optMap;
	std::map<GameState, std::map<Menu::Options, sf::Text>> option;
	std::vector<const Script> scripts;

	bool hardmodeSelected;
	GameState& state;

	sf::String evaluateSpecialChars(sf::String string);
};