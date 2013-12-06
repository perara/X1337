#include "Bullet.h"
#include "GameShape.h"
#include "Log.h"
#include "Enemy.h"
#include "Player.h"
#include "BulletFactory.h"

/// <summary>
/// Initializes a new instance of the <see cref="Bullet"/> class.
/// </summary>
/// <param name="window">The render window.</param>
/// <param name="type">The bullet type.</param>
Bullet::Bullet(sf::RenderWindow& window, Bullet::Type bulletType, const sf::Time& timeStep, std::unique_ptr<ResourceHandler>& resourceHandler) :
Object(window),
bulletType(bulletType),
timeStep(timeStep),
deg(-1) // Set rotation as -1 while not inited.
{

	setDeleted(false);

	if (Bullet::Type::standardShot == bulletType){
		sprite = std::shared_ptr<GameShape>(new GameShape(GameShape::CIRCLE, 3, 10));

	}

	else if (Bullet::Type::heavyShot == bulletType)
	{
		sprite = std::shared_ptr<GameShape>(new GameShape(GameShape::TRIANGLE, 40.0f));
		sprite->setTexture(&resourceHandler->getTexture(ResourceHandler::Texture::HEAVY_SHOT_TEXTURE));
	}

}
/// <summary>
/// Processes this bullet object.
/// </summary>
/// <returns>bool which indicates if the object is up for deletion in Scene.h's object list <see cref="Scene"> </returns>
void Bullet::process()
{
	if (!deleted)
	{
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



		if (isOutOfBounds())
		{
			setDeleted(true);
		}
	}
}

void Bullet::setSpeed(sf::Vector2f speed)
{
	speedX = speed.x;
	speedY = speed.y;
}

void Bullet::setRotation(int degree, sf::Vector2f speed)
{
	deg = degree;
	speedX = speed.x;
	speedY = speed.y;
}


bool Bullet::isOutOfBounds()
{
	if (this->sprite->getPosition().x > window.getView().getSize().x ||
		this->sprite->getPosition().x < 0 ||
		this->sprite->getPosition().y > window.getView().getSize().y ||
		this->sprite->getPosition().y < 0){
		return true;
	}
	else{
		return false;
	}
}

void Bullet::setOwner(Shooter::ShooterType owner)
{

	if (owner == Shooter::ShooterType::PLAYER)
	{
		this->speedX = 0;
		this->speedY = -350;
	}
	else
	{

		this->speedX = 0;
		this->speedY = 150;

	}


	this->owner = owner;
}

void Bullet::resetObject()
{
	this->deg = -1;
	this->sprite->setFillColor(sf::Color::White); // Reset Color;
	this->setDeleted(false);
}

Bullet::Type Bullet::getBulletType()
{
	return this->bulletType;
}

Shooter::ShooterType Bullet::getOwner()
{
	return this->owner;
}
