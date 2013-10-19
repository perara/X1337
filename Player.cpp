#include "Player.h"
#include "GameGlobals.h"

Player::Player(sf::RenderWindow & window, sf::Vector2f pos, int radius) : Object(window){
	this->sprite = new sf::CircleShape(radius,30);
	this->sprite->setPosition(pos);
}

void Player::process(){
	this->inputHandler();
}


void Player::inputHandler(){
	sf::Event event;


	if(sf::Mouse::getPosition(window).x > this->sprite->getRadius() && sf::Mouse::getPosition(window).x < window.getSize().x  - this->sprite->getRadius()){
		this->sprite->setPosition(sf::Mouse::getPosition(window).x - this->sprite->getRadius(), this->sprite->getPosition().y );
	}

	if(sf::Mouse::getPosition(window).y > this->sprite->getRadius() && sf::Mouse::getPosition(window).y < window.getSize().y - this->sprite->getRadius()){
		this->sprite->setPosition(this->sprite->getPosition().x, sf::Mouse::getPosition(window).y  - this->sprite->getRadius());
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