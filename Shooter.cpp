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
			bool wasHit=false;
			if(i->getBulletType()==Bullet::standardShot)
			{
				// HIT DETECTION HERE
				//wasHit = hitDetection(*cs,  *cs2);
			}
			
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

