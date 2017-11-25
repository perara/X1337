#include "../include/Bullet.h"
#include "../include/GameShape.h"
#include "../include/Log.h"
#include "../include/Enemy.h"
#include "../include/Player.h"
#include "../include/BulletFactory.h"


/// <summary>
/// Initializes a new instance of the <see cref="Bullet"/> class.
/// </summary>
/// <param name="window">The window.</param>
/// <param name="bulletType">Type of the bullet.</param>
/// <param name="timeStep">The time step.</param>
/// <param name="resourceHandler">The resource handler.</param>
Bullet::Bullet(sf::RenderWindow& window, Bullet::Type bulletType, const sf::Time& timeStep, std::shared_ptr<ResourceHandler>& resourceHandler) :
Object(window),
bulletType(bulletType),
timeStep(timeStep),
deg(-1) // Set rotation as -1 while not inited.
{
	setDeleted(false);

	// Define the bullet texutres as a standard shot
	if (Bullet::Type::standardShot == bulletType){
		sprite = std::shared_ptr<GameShape>(new GameShape(GameShape::CIRCLE, 3, 10));
		sprite->setTexture(&resourceHandler->getTexture(ResourceHandler::Texture::REGULAR_BULLET_1));
	}

	// Define the bullet textures as a heavy shot 
	else if (Bullet::Type::heavyShot == bulletType)
	{
		sprite = std::shared_ptr<GameShape>(new GameShape(GameShape::TRIANGLE, 40.0f));
		sprite->setTexture(&resourceHandler->getTexture(ResourceHandler::Texture::HEAVY_SHOT_TEXTURE));
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
			double move_x = timeStep.asSeconds() * speedX * cos(deg) - sin(deg);
			double move_y = timeStep.asSeconds() *  speedY * sin(deg) + cos(deg);
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
void Bullet::setOwner(Shooter::ShooterType owner)
{

	// If its a player, define a negative vertical speed
	if (owner == Shooter::ShooterType::PLAYER)
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
	this->owner = owner;
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
Bullet::Type Bullet::getBulletType()
{
	return this->bulletType;
}

/// <summary>
/// Retrieves the owner
/// </summary>
/// <returns>Retrieves the owner value</returns>
Shooter::ShooterType Bullet::getOwner()
{
	return this->owner;
}
