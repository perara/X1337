#pragma once
#include "Object.h"
#include <memory>
#include <SFML\Graphics.hpp>
#include "Powerup.h"

class ResourceHandler;
class Bullet;
class Player;
class BulletFactory;

/// <summary>
/// Powerup is used widely in Script. This class creates a instance of a PowerUp which is later displayed on the screen by a PowerUp list in World
/// </summary>
class Powerup : public Object
{
public:

	/// PowerUpType is an enumerable which contains current implemented Powerup 's 
	const enum PowerUpType
	{
		HEALTH_INCREMENT = -10,
		PULSATING_GUN = -11
	};

	Powerup(
		sf::RenderWindow& window,
		sf::Vector3f startPoint,
		int type,
		std::shared_ptr<ResourceHandler>& resourceHandler,
		const sf::Time& timeStep
		);

	virtual void process();

	bool hitDetection(std::shared_ptr<Player>&);

	Powerup::PowerUpType getPowerUpType();

private:
	int speedX;
	int speedY;

	const sf::Time& timeStep;

	void doAction(std::shared_ptr<Player>& player);

	Powerup::PowerUpType type;

	std::shared_ptr<ResourceHandler>& resourceHandler;

};