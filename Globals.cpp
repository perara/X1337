#include "Globals.h"
#include "ResourceHandler.h"

// Resource Handler getter/setter
void Globals::setResourceHandler(ResourceHandler* rh)
{
	this->resourceHandler = rh;
};

ResourceHandler*  Globals::getResourceHandler()
{
	return this->resourceHandler;
}


// State getter/setter
void Globals::setState(Globals::State state)
{
	this->state = state;
}

Globals::State& Globals::getState()
{
	return this->state;
}

// Time Step getter/setter
void Globals::setTimeStep(sf::Time& timeStep)
{
	this->timeStep = timeStep;
}


sf::Time& Globals::getTimeStep()
{
	return this->timeStep;
}

void Globals::setGameView(sf::View view)
{
	this->gameView = view;
}

sf::View& Globals::getGameView()
{
	return this->gameView;
}
