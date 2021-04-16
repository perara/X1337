#pragma once
#include "Object.h"
#include <memory>
#include <SFML/Graphics.hpp>
#include "VectorN.h"
#include "Constants.h"

class ResourceManager;
class Bullet;
class Player;
class BulletFactory;

/// <summary>
/// Powerup is used widely in Script. This class creates a instance of a PowerUp which is later displayed on the screen by a PowerUp list in World
/// </summary>
class Powerup : public Object
{
public:

	Powerup(
		sf::RenderWindow& window,
		const VectorN& startPoint,
		int type,
		std::shared_ptr<ResourceManager>& resourceHandler,
		const sf::Time& timeStep
		);

	virtual void process();

	bool hitDetection(std::shared_ptr<Player>&);

	Constants::PowerUpType getPowerUpType();

private:
	int speedX;
	int speedY;

	const sf::Time& timeStep;

	void doAction(std::shared_ptr<Player>& player);

    Constants::PowerUpType type;

	std::shared_ptr<ResourceManager>& resourceHandler;

};