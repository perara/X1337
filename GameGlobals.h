#pragma once
#include <iostream>
#include <SFML\Graphics\Font.hpp>
#include "BulletFactory.h"
/*http://stackoverflow.com/questions/270947/can-any-one-provide-me-a-sample-of-singleton-in-c/271104#271104*/
class Config
{
public:
	static Config& getInstance()
	{
		static Config    instance;
		return instance;
	}

	enum State{
		GAME, 
		MENU, 
		OPTIONS, 
		PAUSE
	};
	State state;
	sf::Time elapsedTime;
	sf::Clock gameClock;
	sf::Font font; //Game Font

private:
	Config() {}
	Config(Config const&);              // Don't Implement.
	void operator=(Config const&); // Don't implement
};
