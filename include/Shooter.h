#pragma once
#include "Object.h"
#include "Constants.h"
#include "ResourceManager.h"
#include <memory>

class BulletFactory;
class Bullet;

/// <summary>
/// Shooter is a class which a movable  Object  dervies from.
/// </summary>
class Shooter : public Object
{
public:


	// ShooterType getter
    Constants::ShooterType getType();

	// The Worth of this shooter getter/setter
	void setScoreValue(float);
	float getScoreValue() const;

	// Getters setters for health
	void setHealth(int value);
	int getHealth() const;

    int getStartHealth() const;

    int getTotalDamageDone() const;

protected:
	/// Shooter Constructor which takes in required parameters for the class to work.
	Shooter(Renderer& window,
		BulletFactory&,
		std::list<std::unique_ptr<Bullet>>&,
		std::shared_ptr<ResourceManager>& resourceHandler,
		std::list<std::shared_ptr<Shooter>>& objects,
		const sf::Time& timeStep);

	/// Resource Handler reference originating from GameEngine
	std::shared_ptr<ResourceManager>& resourceHandler;

	// Bullets getter/setter
	std::list<std::unique_ptr<Bullet>>& getBullets();

	/// Timestep variable originating from GameEngine 
	const sf::Time& timeStep;

    /// The Shooter list which contains all enemies (+ player, NB!)
    std::list<std::shared_ptr<Shooter>>& objects;


    // BulletFactory Getter
	BulletFactory& getBulletFactory();

	// Hit Detection processor
	virtual void hitDetection();

	// Getters setters for health
	void setStartHealth(int value);

	/// Shooter type (Player || Enemy)
    Constants::ShooterType shooterType;

	/// Integer which describes startHealth and the totalHealth (since the Shooter starts with 100% health)
	int startHealth;

    void processShipCollision();

private:
	/// Integer value which describes Shooter 's current health
	int health;

	/// Reference to the Bullet list which originates from World.h (World)
	std::list<std::unique_ptr<Bullet>>& bullets;

	/// BulletFactory reference which originates from World.h (World)
	BulletFactory& bFactory;

	/// Shooter 's score value, this value determines the point gain when an Shooter dies (ie Player kills Enemy)
	float scoreValue;

    void addTotalDamageDone(int damage);

    int totalDamageDone;


};