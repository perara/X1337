#include "Powerup.h"
#include "GameShape.h"
#include "ResourceHandler.h"
#include "Player.h"


Powerup::Powerup(
	sf::RenderWindow& window,
	sf::Vector3f startPoint,
	int type,
	std::unique_ptr<ResourceHandler>& resourceHandler,
	const sf::Time& timeStep
	) :
	resourceHandler(resourceHandler),
	type((Powerup::PowerUpType)type),
	Object(window),
	timeStep(timeStep),
	speedX(0),
	speedY(150)
{
	if (type == Powerup::PowerUpType::HEALTH_INCREMENT)
	{
		sprite = std::unique_ptr<GameShape>(new GameShape(GameShape::ShapeType::CIRCLE, 10,20));
		sprite->setTexture(&resourceHandler->getTexture(ResourceHandler::Texture::HEALTH_KIT));
	}
	else if (type == Powerup::PowerUpType::PULSATING_GUN)
	{
		sprite = std::unique_ptr<GameShape>(new GameShape(GameShape::ShapeType::CIRCLE, 20, 20));
		sprite->setTexture(&resourceHandler->getTexture(ResourceHandler::Texture::PULSE_GUN));
	}
	sprite->setPosition(startPoint.x, startPoint.y);
}

void Powerup::process()
{
	if (!deleted)
	{
		this->sprite->setPosition(
			sprite->getPosition().x + (timeStep.asSeconds() * speedX),
			sprite->getPosition().y + (timeStep.asSeconds() * speedY));

	}

	if (isOutOfBounds())
	{
		setDeleted(true);
	}
}

bool Powerup::hitDetection(std::shared_ptr<Player>& player)
{
	bool retVal = sat(this->sprite, player->sprite);

	if (retVal)
	{
		// Set status to deleted
		resourceHandler->getSound(ResourceHandler::Sound::PICKUP_HEALTH).play();
		setDeleted(true);
	}

	return retVal;
}

Powerup::PowerUpType Powerup::getPowerUpType()
{
	return type;
}

