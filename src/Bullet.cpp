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
Bullet::Bullet(sf::RenderWindow& window, Bullet::Type bulletType, const sf::Time& timeStep): 
	Object(window),
	bulletType(bulletType),
	timeStep(timeStep)
{

	setDeleted(false);

	if(Bullet::Type::standardShot == bulletType){
		sprite = std::shared_ptr<GameShape>(new GameShape(GameShape::CIRCLE, 3));

	}

	else if(Bullet::Type::heavyShot == bulletType)
		sprite = std::shared_ptr<GameShape>(new GameShape(GameShape::TRIANGLE, 20.0f));

}
/// <summary>
/// Processes this bullet object.
/// </summary>
/// <returns>bool which indicates if the object is up for deletion in Scene.h's object list <see cref="Scene"> </returns>
void Bullet::process()
{
	if (!deleted)
	{
		this->sprite->setPosition(
			sprite->getPosition().x+(timeStep.asSeconds() * speedX),
			sprite->getPosition().y+(timeStep.asSeconds() * speedY)); //TODO


		if (isOutOfBounds())
		{
			setDeleted(true);
		}
	}
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
