#pragma once
#include "Object.h"
#include "GameGlobals.h"


/// <summary>
/// This class represents a bullet in the game, these bullets will be handled by BulletFactory.h
/// </summary>
class Bullet :  public Object{
public:
	int type;
	Object* owner;
	Bullet(sf::RenderWindow& window, int type);
	bool process();
	void setPosition(int x, int y);
	void setOwner(Object* owner);
	bool deleteQueued;

protected:
	int speedX;
	int speedY;

};

