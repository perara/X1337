#pragma once
#include "Object.h"
#include "Shooter.h"

/// <summary>
/// This class represents a bullet in the game, these bullets will be handled by the BulletFactory
/// </summary>
class Bullet : public Object
{
public:



	Bullet(
	        sf::RenderWindow& window, Constants::BulletType,
	        const sf::Time& timeStep, std::shared_ptr<ResourceManager>& resourceHandler
	        );

	virtual void process();

	// Bullet Reset
	void resetObject();

	// Setters/Getters for properties
    Constants::ShooterType getOwner();
	void setOwner(Constants::ShooterType owner);
	void setRotation(float degree, sf::Vector2f speed);
	Constants::BulletType getBulletType();
	void setSpeed(sf::Vector2f);

private:
    Constants::BulletType bulletType;
	Constants::ShooterType owner;

    float speedX{};
    float speedY{};
	float deg; // Degree on bullet rotation

	const sf::Time& timeStep;
};