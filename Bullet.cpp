#include "Bullet.h"
#include "GameShape.h"
#include "Globals.h"
#include "Log.h"
#include "Enemy.h"
#include "Player.h"
#include "BulletFactory.h"

/// <summary>
/// Initializes a new instance of the <see cref="Bullet"/> class.
/// </summary>
/// <param name="window">The render window.</param>
/// <param name="type">The bullet type.</param>
Bullet::Bullet(sf::RenderWindow& window, Bullet::Type bulletType): 
	Object(window),
	bulletType(bulletType)
{

	this->setDeleted(false);
	this->speedX = 0;
	this->speedY = 0;

	if(Bullet::Type::standardShot == bulletType){
		this->sprite = new GameShape(GameShape::CIRCLE, 2);

	}

	else if(Bullet::Type::heavyShot == bulletType)
		this->sprite = new GameShape(GameShape::TRIANGLE, 20.0f);

}
/// <summary>
/// Processes this bullet object.
/// </summary>
/// <returns>bool which indicates if the object is up for deletion in Scene.h's object list <see cref="Scene"> </returns>
void Bullet::process()
{
	if(!deleted && this->getInited())
	{

		this->sprite->setPosition(
			this->sprite->getPosition().x+(Globals::getInstance().getTimeStep().asSeconds() * speedX),
			this->sprite->getPosition().y+(Globals::getInstance().getTimeStep().asSeconds() * speedY)); //TODO

		if(isOutOfBounds())
		{
			this->setDeleted(true);
		}
	}
}

void Bullet::deleteBullet(BulletFactory& bFactory)
{
	bFactory.returnObject(this);
}
bool Bullet::isOutOfBounds()
{
	if(this->sprite->getPosition().x > window.getSize().x || 
		this->sprite->getPosition().x < 0 || 
		this->sprite->getPosition().y > window.getSize().y || 
		this->sprite->getPosition().y < 0){
			return true;
	}else{
		return false;
	}
}
/// <summary>
/// Sets the position of the bullet.
/// </summary>
/// <param name="x">Sets x position.</param>
/// <param name="y">Sets y position</param>
void Bullet::setPosition(int x, int y)
{
	this->sprite->setPosition(x,y);
}
void Bullet::setOwner(Shooter::ShooterType owner)
{

	if(owner == Shooter::ShooterType::PLAYER)
	{
		this->speedX = 0;
		this->speedY = -250;
	}
	else
	{

		this->speedX = 0;
		this->speedY = 150;

	}


	this->owner = owner;
}

void Bullet::setDeleted(bool val)
{
	this->deleted = val;
}

bool Bullet::getDeleted()
{
	return this->deleted;
}

void Bullet::resetObject()
{
	this->setDeleted(false);
}

Bullet::Type Bullet::getBulletType()
{
	return this->bulletType;
}
