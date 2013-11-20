#include "Shooter.h"

Shooter::Shooter(sf::RenderWindow& window, std::list<Bullet*>& bullets, BulletFactory* bFactory): 
	Object(window),
	bullets(bullets),
	bFactory(bFactory)
{health = 100;}

int Shooter::hitDetection()
{
	// COLLISION TODO
	int hitCounter = 0;
	if(!bullets.empty())
	{
		for(auto& i:bullets)
		{
			bool wasHit;
			if(i->getBulletType()==Bullet::standardShot)
				wasHit = CircleTest(*i->sprite,  *this->sprite);
			
			if(wasHit && this != i->owner)
			{
				i->setDeleted(true);
				health=health-i->getBulletType();
			}
		}
	}
	return hitCounter;
}

void Shooter::setHealth(int value)
{
	this->health = value;
}

int Shooter::getHealth()
{
	return this->health;
}

void Shooter::decrementHealth()
{
	this->health--;
}

void Shooter::incrementHealth()
{
	this->health++;
}

void Shooter::shootableProcess()
{
	int hitCount = this->hitDetection();

	if(hitCount > 0)
	{
		//LOGD("Shootable#"<<this << " has " << this->getHealth() << " health");
		this->decrementHealth();
	}
}

/* Privates */
sf::Vector2f Shooter::GetSpriteCenter (const sf::CircleShape& Object)
{
	sf::FloatRect AABB = Object.getGlobalBounds();
	return sf::Vector2f (AABB.left+AABB.width/2.f, AABB.top+AABB.height/2.f);
}

sf::Vector2f Shooter::GetSpriteSize (const sf::CircleShape& Object)
{
	sf::IntRect OriginalSize = Object.getTextureRect();
	sf::Vector2f Scale = Object.getScale();
	return sf::Vector2f (OriginalSize.width*Scale.x, OriginalSize.height*Scale.y);
}

bool Shooter::CircleTest(const sf::CircleShape& Object1, const sf::CircleShape& Object2) 
{
	sf::Vector2f Obj1Size = GetSpriteSize(Object1);
	sf::Vector2f Obj2Size = GetSpriteSize(Object2);
	float Radius1 = Object1.getRadius();
	float Radius2 = Object2.getRadius();

	sf::Vector2f Distance = GetSpriteCenter(Object1)-GetSpriteCenter(Object2);

	return (Distance.x * Distance.x + Distance.y * Distance.y <= (Radius1 + Radius2) * (Radius1 + Radius2));
}