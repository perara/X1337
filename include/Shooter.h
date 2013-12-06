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
		std::unique_ptr<ResourceHandler>& resourceHandler,
		const sf::Time& timeStep);
	// Resource Handler reference
	std::unique_ptr<ResourceHandler>& resourceHandler;


	// Set the type of this shooter (Enemy || Player)
	void setType(Shooter::ShooterType);

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
	const int getHealth(); // Todo should be protected
private:
	int health;

	// Hyperplane separation theorem for hit detection
	bool sat(std::shared_ptr<GameShape> c1, std::shared_ptr<GameShape> c2);

	// Shooter type (Player || Enemy)
	ShooterType shooterType;

	// References to bullets and bulletfactory
	std::list<std::unique_ptr<Bullet>>& bullets;
	BulletFactory& bFactory;

	// Shooter's Worth (Value)
	float scoreValue;
};