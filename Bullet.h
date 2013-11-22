#pragma once
#include "BulletFactory.h"
#include "Object.h"


/// <summary>
/// This class represents a bullet in the game, these bullets will be handled by BulletFactory.h
/// </summary>
class Bullet :  public Object
{
public:

	Object* owner;
	Bullet(sf::RenderWindow& window, BulletFactory::BulletType bulletType);
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