#pragma once
#include "Object.h"
#include "Shooter.h"

/// <summary>
/// This class represents a bullet in the game, these bullets will be handled by the BulletFactory
/// </summary>
class Bullet : public Object
{
public:

	/// Defines the Bullet Type in an enumerable. This is all implemented bullets in the Game.
	const enum Type {
		standardShot = 1,
		heavyShot = 5
	};

	Bullet(sf::RenderWindow& window, Bullet::Type, const sf::Time& timeStep, std::shared_ptr<ResourceHandler>& resourceHandler);

	virtual void process();

	// Bullet Resetter
	void resetObject();

	// Setters/Getters for properties
	Shooter::ShooterType getOwner();
	void setOwner(Shooter::ShooterType owner);
	void setRotation(int degree, sf::Vector2f speed);
	Bullet::Type getBulletType();
	void setSpeed(sf::Vector2f);

private:
	Bullet::Type bulletType;
	Shooter::ShooterType owner;

	int speedX;
	int speedY;
	int deg; // Degree on bullet rotation

	const sf::Time& timeStep;
};