#pragma once
#include "Scene.h"
#include "ResourceManager.h"
#include "Constants.h"
#include <SFML/Graphics.hpp>
#include <map>
#include <list>

/// <summary>
/// A class which is called by GameEngine. This class serves the menu for the whole application. It is based on GameState
/// </summary>
class Menu : public Scene{
public:

	Menu(Renderer& window, GameState& state, std::shared_ptr<ResourceManager>& resourceHandler);

	// Scene virtuals
	virtual void draw();
	virtual void process();
	virtual void input(sf::Event&);



	void init();
	void reset();
	bool getHardmodeSelected() const;
	void resetCurrentOption();
	void drawPause(float xOffSet, float yOffset); // Should be accessible to gameEngine
	int getStageSelectOption() const;
	void setStageSelectOption(int);
	void setMessageOfTheDayId(int);
private:
	// Loaders
	void loadMenuOptions();


	// Draw Functions
	void drawStageSelect();
	void drawMainMenu();
	void drawOptions(GameState, float xOffset = 0, float yOffset = 0, sf::Color color = sf::Color(139, 137, 137));
	void drawGameTitle();
	void drawHighScore();
	void drawCredits();

	// Input functions
	void stageSelectInput(sf::Event&);




	int currentOption; // Current selected menu option
	int getCurrentOption() const;
	void setCurrentOption(int);

	int numStages; // This describes number of stages
	int stageSelectOption; // Current selected stage option
	int messageOfTheDayId; //Current message of the day id (From ResourceHandler)

	// Option lists
	std::map<GameState, std::map<Constants::MenuC::Options, std::string>> optMap;
	std::map<GameState, std::map<Constants::MenuC::Options, sf::Text>> option;
	std::vector<ScriptTemplate> scripts;

	bool hardmodeSelected;
	GameState& state;

	static sf::String evaluateSpecialChars(sf::String string);
};