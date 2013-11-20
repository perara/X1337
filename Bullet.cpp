#include "Bullet.h"

/// <summary>
/// Initializes a new instance of the <see cref="Bullet"/> class.
/// </summary>
/// <param name="window">The render window.</param>
/// <param name="type">The bullet type.</param>
Bullet::Bullet(sf::RenderWindow& window, BulletFactory::BulletType bulletType, std::list<Bullet*>& bullets, BulletFactory* bFactory): 
	Object(window),
	bulletType(bulletType),
	bullets(bullets),
	bFactory(bFactory)
{

	this->setDeleted(false);
	this->speedX = 0;
	this->speedY = -500;
	this->sprite = new sf::CircleShape(2,30);
}
/// <summary>
/// Processes this bullet object.
/// </summary>
/// <returns>bool which indicates if the object is up for deletion in Scene.h's object list <see cref="Scene"> </returns>
bool Bullet::process()
{
	if(!deleted)
	{
		this->sprite->setPosition(
			this->sprite->getPosition().x+(Config::getInstance().elapsedTime.asSeconds() * speedX),
			this->sprite->getPosition().y+(Config::getInstance().elapsedTime.asSeconds() * speedY)); //TODO

		if(isOutOfBounds())
		{
			this->setDeleted(true);
		}
	}
	return true; // TODODODODODODO
}
void Bullet::deleteBullet(std::list<Bullet*>::iterator i)
{
	this->bFactory->returnObject(this);
	bullets.erase(i);
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
void Bullet::setOwner(Object* owner)
{
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
BulletFactory::BulletType Bullet::getBulletType()
{
	return this->bulletType;
}