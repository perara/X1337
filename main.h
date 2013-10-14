#pragma once
#include <iostream>
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
private:
	Config() {}
	Config(Config const&);              // Don't Implement.
	void operator=(Config const&); // Don't implement
};
