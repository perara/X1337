#include "InputController.h"

void InputController::updateWorld()
{
	sf::Event event;

	if(sf::Mouse::getPosition(window).x>0
		&& sf::Mouse::getPosition(window).x<w.windowSize.x-(w.p1.playerShape.getRadius()*2))
			w.p1.setPlayerPosX(sf::Mouse::getPosition(window).x);

	if(sf::Mouse::getPosition(window).y>0
		&& sf::Mouse::getPosition(window).y<w.windowSize.y-(w.p1.playerShape.getRadius()*2))
			w.p1.setPlayerPosY(sf::Mouse::getPosition(window).y);

	if(sf::Mouse::isButtonPressed)
	{

	}

	while (window.pollEvent(event))
    {
		if (event.type == sf::Event::Closed)
			window.close();
	}
}

InputController::InputController(World& w, sf::Window& window):
	w(w),
	window(window)
{
}


InputController::~InputController(void)
{
}
