#include "Bullet.h"

/// <summary>
/// Initializes a new instance of the <see cref="Bullet"/> class.
/// </summary>
/// <param name="window">The render window.</param>
/// <param name="type">The bullet type.</param>
Bullet::Bullet(sf::RenderWindow& window, int type, std::list<Bullet*>& bullets, BulletFactory* bFactory): 
	Object(window),
	type(type),
	bullets(bullets),
	bFactory(bFactory)
{
	this->speedX = 0;
	this->speedY = -100;
	this->sprite = new sf::CircleShape(2,30);
}

/// <summary>
/// Processes this bullet object.
/// </summary>
/// <returns>bool which indicates if the object is up for deletion in Scene.h's object list <see cref="Scene"> </returns>
bool Bullet::process(){
	this->sprite->setPosition(
		this->sprite->getPosition().x+(Config::getInstance().elapsedTime.asSeconds() * speedX),
		this->sprite->getPosition().y+(Config::getInstance().elapsedTime.asSeconds() * speedY)); //TODO
	if(isOutOfBounds)
	{
		deleteBullet();
	}
}

void Bullet::deleteBullet()
{
	this->bFactory->returnObject(this);
	std::list<Bullet*>::iterator i = std::find(bullets.begin(), bullets.end(), this);
	bullets.erase(i);
}
bool Bullet::isOutOfBounds()
{
	if(this->sprite->getPosition.z>window.getSize().x || this->sprite->getPosition.x<0 || this->sprite->getPosition.y>window.getSize().y || this->sprite->getPosition.y<0)
		return true;
	else
		return false;
}


/// <summary>
/// Sets the position of the bullet.
/// </summary>
/// <param name="x">Sets x position.</param>
/// <param name="y">Sets y position</param>
void Bullet::setPosition(int x, int y){
	this->sprite->setPosition(x,y);
}

void Bullet::setOwner(Object* owner){
	this->owner = owner;
}