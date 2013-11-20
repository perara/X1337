#pragma once
#include <iostream>

#include <SFML\System.hpp>
#include <SFML\Graphics\Font.hpp>
/*http://stackoverflow.com/questions/270947/can-any-one-provide-me-a-sample-of-singleton-in-c/271104#271104*/
/// <summary>
/// A singleton which eases use of some variables. We use this to pass objects which we see as unnecessary to pass via Dependency Injection.
/// </summary>
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
