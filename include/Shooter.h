#pragma once
#include "Object.h"
#include "ResourceHandler.h"
#include <memory>

class BulletFactory;
class Bullet;

/// <summary>
/// Shooter is a class which a movable  Object  dervies from.
/// </summary>
class Shooter : public Object
{
public:

	/// An enumration which describes the ShooterType
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

	// Getters setters for health
	void setHealth(int value);
	const int getHealth();
protected:
	/// Shooter Constructor which takes in required parameters for the class to work.
	Shooter(sf::RenderWindow& window,
		BulletFactory&,
		std::list<std::unique_ptr<Bullet>>&,
		std::shared_ptr<ResourceHandler>& resourceHandler,
		const sf::Time& timeStep);

	/// Resource Handler reference originating from GameEngine
	std::shared_ptr<ResourceHandler>& resourceHandler;

	// Bullets getter/setter
	std::list<std::unique_ptr<Bullet>>& getBullets();

	/// Timestep variable originating from GameEngine 
	const sf::Time& timeStep;

	// BulletFactory Getter
	BulletFactory& getBulletFactory();

	// Hit Detection processor
	virtual void hitDetection();

	// Getters setters for health
	const int getStartHealth();
	void setStartHealth(int value);

	/// Shooter type (Player || Enemy)
	ShooterType shooterType;

	/// Integer which describes startHealth and the totalHealth (since the Shooter starts with 100% health)
	int startHealth;

private:
	/// Integer value which describes Shooter 's current health
	int health;

	/// Reference to the Bullet list which originates from World.h (World)
	std::list<std::unique_ptr<Bullet>>& bullets;

	/// BulletFactory reference which originates from World.h (World)
	BulletFactory& bFactory;

	/// Shooter 's score value, this value determines the point gain when an Shooter dies (ie Player kills Enemy)
	float scoreValue;
};