#pragma once
#include "Object.h"
#include "Shooter.h"

/// <summary>
/// This class represents a bullet in the game, these bullets will be handled by BulletFactory.h
/// </summary>
class Bullet : public Object
{
public:

	const enum Type {
		standardShot = 1,
		heavyShot = 5
	};

	Bullet(sf::RenderWindow& window, Bullet::Type,  const sf::Time& timeStep);

	virtual void process();

	void resetObject();

	Shooter::ShooterType getOwner();
	void setOwner(Shooter::ShooterType owner);	
	
	Bullet::Type getBulletType();

private:	

	Bullet::Type bulletType;
	Shooter::ShooterType owner;

	int speedX;
	int speedY;

	const sf::Time& timeStep;

	bool isOutOfBounds();
};