#include "Player.h"
#include "GameGlobals.h"
#include <SFML\System\Clock.hpp>
#include <functional>
sf::Clock clk; //TESTING PURPOSES



/// <summary>
/// Initializes a new instance of the <see cref="Player"/> class.
/// </summary>
/// <param name="window">The render window.</param>
/// <param name="pos">The initial startposition of the player</param>
/// <param name="radius">The radius.</param>
/// <param name="bFactory">The <see cref=BulletFactory"></param>
/// <param name="sceneObjCallBack">The scene object call back. This is basicly a function pointer to the corresponding world function "addObject" Reason for passing this is so we can add bullets to the Scene loop</param>
Player::Player(sf::RenderWindow& window, sf::Vector2f pos, int radius, BulletFactory* bFactory, std::function<void(Object*)> sceneObjCallBack):
	bFactory(bFactory),
	sceneObjCallBack(sceneObjCallBack),
	Object(window)
{

	this->sprite = new sf::CircleShape(radius,30);
	this->sprite->setPosition(pos);



}

bool Player::process(){
	this->inputHandler();
	return true;
}


void Player::inputHandler(){
	sf::Event event;


	if(sf::Mouse::getPosition(window).x > this->sprite->getRadius() && sf::Mouse::getPosition(window).x < window.getSize().x  - this->sprite->getRadius()){
		this->sprite->setPosition(sf::Mouse::getPosition(window).x - this->sprite->getRadius(), this->sprite->getPosition().y );
	}

	if(sf::Mouse::getPosition(window).y > this->sprite->getRadius() && sf::Mouse::getPosition(window).y < window.getSize().y - this->sprite->getRadius()){
		this->sprite->setPosition(this->sprite->getPosition().x, sf::Mouse::getPosition(window).y  - this->sprite->getRadius());
	}



	if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && clk.getElapsedTime().asMilliseconds() > 0){
		Bullet* b = this->bFactory->requestObject(1);
		b->setPosition(this->sprite->getPosition().x , this->sprite->getPosition().y);
		this->sceneObjCallBack(b);
		clk.restart();
	}


	// Game Menu
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
		std::cout << "HEHE" << std::endl;
		Config::getInstance().state = Config::getInstance().MENU;
	}



	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();



	}


}