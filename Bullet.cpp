#include "Bullet.h"

Bullet::Bullet(sf::RenderWindow& window, int type): Object(window)
{
	this->speedX = 20;
	this->speedY = -2000;
	this->type = type;
	this->sprite = new sf::CircleShape(2,30);
}

bool Bullet::process(){
	if(this->sprite->getPosition().y > 0){
		this->sprite->setPosition(
			this->sprite->getPosition().x+(Config::getInstance().elapsedTime.asSeconds()* speedX),
			this->sprite->getPosition().y+(Config::getInstance().elapsedTime.asSeconds() * speedY)); //TODO lol
		return true;
	}
	return false;
}

void Bullet::setPosition(int x, int y){
	this->sprite->setPosition(x,y);
}