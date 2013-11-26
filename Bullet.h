#pragma once
#include "Object.h"
#include "Shooter.h"

/// <summary>
/// This class represents a bullet in the game, these bullets will be handled by BulletFactory.h
/// </summary>
class Bullet :  public Object
{
public:
	const enum Type {
		standardShot = 1,
		heavyShot = 5
	};

	Shooter::ShooterType owner;
	Bullet(sf::RenderWindow& window, Bullet::Type);
	void process();
	bool isOutOfBounds();

	void setPosition(int x, int y);
	void setOwner(Shooter::ShooterType owner);
	void deleteBullet(BulletFactory& bFactory);
	void resetObject();
	void setDeleted(bool deleted);
	bool getDeleted(); 

	Bullet::Type getBulletType();

protected:
	Bullet::Type bulletType;
	int speedX;
	int speedY;
	bool deleted;
};