#pragma once
#include "Object.h"


class Bullet :  public Object{
public:
	int type;
	Bullet(sf::RenderWindow& window, int type): Object(window)
	{
		this->type = type;
		this->sprite = new sf::CircleShape(2,30);
	};

	bool process(){
		if(this->sprite->getPosition().y > 0){
			this->sprite->setPosition(
				this->sprite->getPosition().x+(Config::getInstance().elapsedTime.asSeconds()*speedX),
				this->sprite->getPosition().y+(Config::getInstance().elapsedTime.asSeconds()*speedY)); //TODO lol
			return true;
		}
		return false;
	}

	void setPosition(int x, int y){
		this->sprite->setPosition(x,y);
	}

private:
	int speedX;
	int speedY;

};

