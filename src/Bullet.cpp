#include "../include/GameShape.h"
#include "../include/BulletFactory.h"
#include <cmath>
#include <memory>


/// <summary>
/// Initializes a new instance of the <see cref="Bullet"/> class.
/// </summary>
/// <param name="window">The window.</param>
/// <param name="bulletType">Type of the bullet.</param>
/// <param name="timeStep">The time step.</param>
/// <param name="resourceHandler">The resource handler.</param>
Bullet::Bullet(sf::RenderWindow& window, Constants::BulletType bulletType, const sf::Time& timeStep, std::shared_ptr<ResourceManager>& resourceHandler) :
Object(window),
owner(Constants::ShooterType::NONE),
bulletType(bulletType),
timeStep(timeStep),

deg(-1) // Set rotation as -1 while not inited.
{
	setDeleted(false);

	// Define the bullet texutres as a standard shot
	if (Constants::BulletType::standardShot == bulletType){
		sprite = std::make_shared<GameShape>(Constants::GameShapeC::Type::CIRCLE, 3.0f, 10);
		sprite->setTexture(&resourceHandler->getTexture(Constants::ResourceC::Texture::REGULAR_BULLET_1));
	}

	// Define the bullet textures as a heavy shot 
	else if (Constants::BulletType::heavyShot == bulletType)
	{
		sprite = std::make_shared<GameShape>(Constants::GameShapeC::Type::TRIANGLE, 40.0f);
		sprite->setTexture(&resourceHandler->getTexture(Constants::ResourceC::Texture::HEAVY_SHOT_TEXTURE));
	}

}


/// <summary>
/// Processes the bullet
/// </summary>
void Bullet::process()
{

	// Checks if its deleted
	if (!getDeleted())
	{

		// If degrees is set (not -1), do rotational movement
		if (deg != -1) // Rotational movement
		{
			float move_x = timeStep.asSeconds() * speedX * std::cos(deg) - std::sin(deg);
            float move_y = timeStep.asSeconds() *  speedY * std::sin(deg) + std::cos(deg);
			this->sprite->move(move_x, move_y);
		}
		else // Linear movement.
		{
			this->sprite->setPosition(
				sprite->getPosition().x + (timeStep.asSeconds() * speedX),
				sprite->getPosition().y + (timeStep.asSeconds() * speedY));

		}

		// If the bullet is out of bounds, we set delete flag
		if (isOutOfBounds())
		{
			setDeleted(true);
		}
	}
}

/// <summary>
/// Sets the speed of the bullet
/// </summary>
/// <param name="speed">The speed of the bullet</param>
void Bullet::setSpeed(sf::Vector2f speed)
{
	speedX = speed.x;
	speedY = speed.y;
}

/// <summary>
/// Sets the rotation angle of the bullet
/// </summary>
/// <param name="degree">The degree of the bullet</param>
/// <param name="speed">The speed. of the bullet</param>
void Bullet::setRotation(float degree, sf::Vector2f speed)
{
	deg = degree;
	speedX = speed.x;
	speedY = speed.y;
}



/// <summary>
/// Sets the Shootertype owner of the bullet
/// </summary>
/// <param name="owner">The owner.</param>
void Bullet::setOwner(Constants::ShooterType _owner)
{

	// If its a player, define a negative vertical speed
	if (_owner == Constants::ShooterType::PLAYER)
	{
		this->speedX = 0;
		this->speedY = -350;
	}

	// If its somthing else (enemy) positive vertical speed.
	else
	{

		this->speedX = 0;
		this->speedY = 150;

	}

	// Set the owner
	this->owner = _owner;
}

/// <summary>
/// Resets the object properties.
/// </summary>
void Bullet::resetObject()
{
	this->deg = -1;
	this->sprite->setFillColor(sf::Color::White); // Reset Color;
	this->setDeleted(false);
}

/// <summary>
/// Retrieves bullet type
/// </summary>
/// <returns>Bullet type</returns>
Constants::BulletType Bullet::getBulletType()
{
	return this->bulletType;
}

/// <summary>
/// Retrieves the owner
/// </summary>
/// <returns>Retrieves the owner value</returns>
Constants::ShooterType Bullet::getOwner()
{
	return this->owner;
}
