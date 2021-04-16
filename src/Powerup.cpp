#include "../include/Powerup.h"

#include <memory>
#include "../include/ResourceManager.h"
#include "../include/Player.h"
#include "../include/GameShape.h"

/// <summary>
/// Initializes a new instance of the <see cref="Powerup"/> class.
/// </summary>
/// <param name="window">The window.</param>
/// <param name="startPoint">The start point.</param>
/// <param name="type">The type.</param>
/// <param name="resourceHandler">The resource handler.</param>
/// <param name="timeStep">The time step.</param>
Powerup::Powerup(
	sf::RenderWindow& window,
	const VectorN& startPoint,
	int type,
	std::shared_ptr<ResourceManager>& resourceHandler,
	const sf::Time& timeStep
	) :
	resourceHandler(resourceHandler),
	type((Constants::PowerUpType)type),
	Object(window),
	timeStep(timeStep),
	speedX(0),
	speedY(150)
{
	// Determine which type the poweup is, set the desired texture.
	if (type == Constants::PowerUpType::HEALTH_INCREMENT)
	{
		sprite = std::make_unique<GameShape>(Constants::GameShapeC::Type::CIRCLE, 10, 20);
		sprite->setTexture(&resourceHandler->getTexture(Constants::ResourceC::Texture::HEALTH_KIT));
	}
	else if (type == Constants::PowerUpType::PULSATING_GUN)
	{
		sprite = std::make_unique<GameShape>(Constants::GameShapeC::Type::CIRCLE, 20, 20);
		sprite->setTexture(&resourceHandler->getTexture(Constants::ResourceC::Texture::PULSE_GUN));
	}

	// Set the position of the sprite powerup
	sprite->setPosition(startPoint.x, startPoint.y);
}

/// <summary>
/// Processes the powerup
/// </summary>
void Powerup::process()
{

	// Check if the powerup is deleted or not, if it is, we do not want to move it.
	if (!getDeleted())
	{
		this->sprite->setPosition(
			sprite->getPosition().x + (timeStep.asSeconds() * speedX),
			sprite->getPosition().y + (timeStep.asSeconds() * speedY));

	}

	// Check if the powerup is out of bounds
	if (isOutOfBounds())
	{
		setDeleted(true);
	}
}

/// <summary>
/// A function which determines weither the player and a powerup collided.
/// </summary>
/// <param name="player">The player.</param>
/// <returns>Bool value which determines a pickup or not</returns>
bool Powerup::hitDetection(std::shared_ptr<Player>& player)
{
	// CHeck hit detection
	bool retVal = sat(this->sprite, player->sprite);

	// Check if there was a hit collision
	if (retVal)
	{
		// Set status to deleted
		resourceHandler->getSound(Constants::ResourceC::Sound::FX_PICKUP_HEALTH).play();
		setDeleted(true);
	}

	return retVal;
}

/// <summary>
/// Gets the type of the power up.
/// </summary>
/// <returns>Returns the type of the power up</returns>
Constants::PowerUpType Powerup::getPowerUpType()
{
	return type;
}

