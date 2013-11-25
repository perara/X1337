#pragma once
#include "Scene.h"
#include <SFML\Graphics.hpp>

class Menu: public Scene{
public:

	Menu(sf::RenderWindow& window);
	~Menu();

	virtual void draw();
	virtual void process();
	virtual void init();
	virtual void input(sf::Event&);
	virtual void reset();

private:
	void loadMenuOptions();
	

	const enum Options
	{
		NEW_GAME,
		LOAD_GAME,
		CREDITS,
		EXIT_GAME,
		MENU_OPT_COUNT
	};

	int currentOption;

	int getCurrentOption();
	void setCurrentOption(int);


	// Option lists
	std::map<Options, std::string> optList;
	sf::Text* option[Options::MENU_OPT_COUNT];

};