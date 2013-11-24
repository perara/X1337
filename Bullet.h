#pragma once
#include "BulletFactory.h"
#include "Object.h"
#include "Shooter.h"

/// <summary>
/// This class represents a bullet in the game, these bullets will be handled by BulletFactory.h
/// </summary>
class Bullet :  public Object
{
public:

	Shooter::ShooterType owner;
	Bullet(sf::RenderWindow& window, BulletFactory::BulletType bulletType);
	void process();
	bool isOutOfBounds();
	
	void setPosition(int x, int y);
	void setOwner(Shooter::ShooterType owner);
	void deleteBullet();
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