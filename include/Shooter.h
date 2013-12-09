#pragma once
#include "Object.h"
#include "ResourceHandler.h"
#include <memory>

class BulletFactory;
class Bullet;

class Shooter : public Object
{
public:
	const enum ShooterType
	{
		PLAYER,
		ENEMY
	};

	// ShooterType getter
	Shooter::ShooterType getType();

	// The Worth of this shooter getter/setter
	void setScoreValue(float);
	float getScoreValue();

protected:

	Shooter(sf::RenderWindow& window,
		BulletFactory&,
		std::list<std::unique_ptr<Bullet>>&,
		std::shared_ptr<ResourceHandler>& resourceHandler,
		const sf::Time& timeStep);
	// Resource Handler reference
	std::shared_ptr<ResourceHandler>& resourceHandler;

	// Bullets getter/setter
	std::list<std::unique_ptr<Bullet>>& getBullets();

	// Game Timestep flow
	const sf::Time& timeStep;

	// BulletFactory Getter
	BulletFactory& getBulletFactory();

	// Hit Detection processor
	virtual void hitDetection();

	// Getters setters for health
	void setHealth(int value);
	const int getHealth();
	const int getStartHealth();
	void setStartHealth(int value);

	// Shooter type (Player || Enemy)
	ShooterType shooterType;

private:
	int health;
	int startHealth;

	// References to bullets and bulletfactory
	std::list<std::unique_ptr<Bullet>>& bullets;
	BulletFactory& bFactory;

	// Shooter's Worth (Value)
	float scoreValue;
};