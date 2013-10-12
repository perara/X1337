#include "PlayerController.h"

void PlayerController::updateWorld()
{
	sf::Event event;

	if(sf::Mouse::getPosition(window).x>0
		&& sf::Mouse::getPosition(window).x<w.windowSize.x-(player.playerShape.getRadius()*2))
			player.setPlayerPosX(sf::Mouse::getPosition(window).x);

	if(sf::Mouse::getPosition(window).y>0
		&& sf::Mouse::getPosition(window).y<w.windowSize.y-(player.playerShape.getRadius()*2))
			player.setPlayerPosY(sf::Mouse::getPosition(window).y);

	if(sf::Mouse::isButtonPressed)
	{

	}

	while (window.pollEvent(event))
    {
		if (event.type == sf::Event::Closed)
			window.close();
	}
}

PlayerController::PlayerController(Window& w,Player& player, sf::Window& window):
	w(w),
	player(player),
	window(window)
{
}


PlayerController::~PlayerController(void)
{
}
