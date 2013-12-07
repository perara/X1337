#pragma once
#include "Object.h"
#include <memory>
#include <SFML\Graphics.hpp>
#include "Powerup.h"

class ResourceHandler;
class Bullet;
class Player;
class BulletFactory;

/* Reason for PowerUP beeing an enemy is movement.
This enemy will not be able to shoot.*/
class Powerup : public Object
{
public:

	const enum PowerUpType
	{
		HEALTH_INCREMENT = -10
	};


	Powerup(
	sf::RenderWindow& window,
	sf::Vector3f startPoint,
	int type,
	std::unique_ptr<ResourceHandler>& resourceHandler,
	const sf::Time& timeStep
	);

	void process();
	bool hitDetection(std::shared_ptr<Player>&);
	Powerup::PowerUpType getPowerUpType();

private:
	int speedX;
	int speedY;
	const sf::Time& timeStep;
	void doAction(std::shared_ptr<Player>& player);
	Powerup::PowerUpType type;
	std::unique_ptr<ResourceHandler>& resourceHandler;

};