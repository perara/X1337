#include "Bullet.h"

/// <summary>
/// Initializes a new instance of the <see cref="Bullet"/> class.
/// </summary>
/// <param name="window">The render window.</param>
/// <param name="type">The bullet type.</param>
Bullet::Bullet(sf::RenderWindow& window, int type): Object(window)
{
	this->speedX = 20;
	this->speedY = -500;
	this->type = type;
	this->sprite = new sf::CircleShape(2,30);
}

/// <summary>
/// Processes this bullet object.
/// </summary>
/// <returns>bool which indicates if the object is up for deletion in Scene.h's object list <see cref="Scene"> </returns>
bool Bullet::process(){
	if(this->sprite->getPosition().y > 0){
		this->sprite->setPosition(
			this->sprite->getPosition().x+(Config::getInstance().elapsedTime.asSeconds() * speedX),
			this->sprite->getPosition().y+(Config::getInstance().elapsedTime.asSeconds() * speedY)); //TODO
		return true;
	}
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

