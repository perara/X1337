#pragma once
#include <SFML\System.hpp>
#include <SFML\Graphics\Font.hpp>

class ResourceHandler;


/*http://stackoverflow.com/questions/270947/can-any-one-provide-me-a-sample-of-singleton-in-c/271104#271104*/
/// <summary>
/// A singleton which eases use of some variables. We use this to pass objects which we see as unnecessary to pass via Dependency Injection.
/// </summary>
class Globals
{
public:
	static Globals& getInstance()
	{
		static Globals    instance;
		return instance;
	}

	enum State{
		GAME, 
		MENU, 
		OPTIONS, 
		PAUSE
	};

	// Resource Handler getter/setter
	void setResourceHandler(ResourceHandler* rh);
	ResourceHandler* getResourceHandler();

	// State getter/setter
	void setState(Globals::State state);
	Globals::State& getState();

	// Time Step getter/setter
	void setTimeStep(sf::Time& timeStep);
	sf::Time& getTimeStep();

	int statsltakts;


private:
	ResourceHandler* resourceHandler;
	State state;
	sf::Time timeStep;

private:
	Globals() {}
	Globals(Globals const&);        // Don't Implement.
	void operator=(Globals const&); // Don't implement


};
