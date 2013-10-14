#include "Player.h"
#include "main.h"

Player::Player(sf::Vector2f pos, int radius){
	this->sprite = new sf::CircleShape(radius,30);
	this->sprite->setPosition(pos);
}

void Player::inputHandler(sf::RenderWindow &window){
	sf::Event event;

	if(sf::Mouse::getPosition(window).x > 0 && sf::Mouse::getPosition(window).x < window.getSize().x - (this->sprite->getRadius() * 2)){
		this->sprite->setPosition(sf::Mouse::getPosition(window).x, this->sprite->getPosition().y);
	}

	if(sf::Mouse::getPosition(window).y > 0 && sf::Mouse::getPosition(window).y < window.getSize().y - (this->sprite->getRadius() * 2)){
		this->sprite->setPosition(this->sprite->getPosition().x , sf::Mouse::getPosition(window).y);
	}

	if(sf::Mouse::isButtonPressed)
	{

	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
		std::cout << "LOL" << std::endl;
		Config::getInstance().state = Config::getInstance().MENU;
	}

	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
	}


}