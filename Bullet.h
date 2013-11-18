#pragma once
#include "Object.h"
#include "GameGlobals.h"
#include <list>
#include "BulletFactory.h"


/// <summary>
/// This class represents a bullet in the game, these bullets will be handled by BulletFactory.h
/// </summary>
class Bullet :  public Object{
public:
	int type;
	Object* owner;
	BulletFactory* bFactory;
	std::list<Bullet*>& bullets;
	Bullet(sf::RenderWindow& window, int type, std::list<Bullet*>& bullets, BulletFactory* bFactory);
	bool process();
	void setPosition(int x, int y);
	void setOwner(Object* owner);
	bool isOutOfBounds();
	void deleteBullet();

protected:
	int speedX;
	int speedY;

};

