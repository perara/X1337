#pragma once
#include "Object.h"


class Bullet :  public Object{
public:
	Bullet(sf::RenderWindow& window, int type): Object(window)
	{
		this->sprite = new sf::CircleShape(2,30);
	};

	void process(){
		this->sprite->setPosition(this->sprite->getPosition().x, this->sprite->getPosition().y -1); //TODO lol

	}

	void setPosition(int x, int y){
		this->sprite->setPosition(x,y);
	}

};

