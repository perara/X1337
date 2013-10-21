#pragma once
#include "Object.h"
#include "GameGlobals.h"


class Bullet :  public Object{
public:
	int type;

	Bullet(sf::RenderWindow& window, int type);
	bool process();
	void setPosition(int x, int y);

private:
	int speedX;
	int speedY;

};

