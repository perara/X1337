#pragma once
#include "Object.h"
#include "GameGlobals.h"
#include <list>
#include "BulletFactory.h"


/// <summary>
/// This class represents a bullet in the game, these bullets will be handled by BulletFactory.h
/// </summary>
class Bullet :  public Object
{
public:

	Object* owner;
	BulletFactory* bFactory;
	std::list<Bullet*>& bullets;
	Bullet(sf::RenderWindow& window, BulletFactory::BulletType bulletType, std::list<Bullet*>& bullets, BulletFactory* bFactory);
	void process();
	bool isOutOfBounds();
	
	void setPosition(int x, int y);
	void setOwner(Object* owner);
	void deleteBullet(std::list<Bullet*>::iterator i);
	void resetObject();
	void setDeleted(bool deleted);
	bool getDeleted(); 
	BulletFactory::BulletType getBulletType();

protected:
	BulletFactory::BulletType bulletType;
	int speedX;
	int speedY;
	bool deleted;
};