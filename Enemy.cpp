#include "Enemy.h"
#include "stdlib.h"
sf::Clock clsk;

Enemy::Enemy(sf::RenderWindow& window, 
			 sf::Vector2f startPos,
			 sf::Vector2f endPos,
			 float shootFrequency,
			 int radius, 
			 BulletFactory* bFactory, 
			 std::list<Bullet*>& bullets):
startPos(startPos),
	endPos(endPos),
	Shooter(window, bullets, bFactory)
{
	this->sprite = new GameShape(GameShape::circle, 10);
	this->sprite->setPosition(startPos);
}
int Enemy::hitDetection()
{
	// COLLISION TODO
	int hitCounter = 0;
	if(!bullets.empty())
	{
		for(auto& i:bullets)
		{
			bool wasHit;
			if(i->getBulletType() == BulletFactory::BulletType::standardShot)
			{
				wasHit = circleTest(*i->sprite);
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

bool Enemy::process()
{
	this->shootableProcess();
	if((int)this->sprite->getPosition().x != (int)endPos.x || (int)this->sprite->getPosition().y != (int)endPos.y){
		float angle = atan2f(endPos.x - startPos.x, endPos.y - startPos.y) * 180 / 3.14;
		float x = sin(angle)*0.1f;
		float y = cos(angle)*0.1f;
		this->sprite->move(x + (Config::getInstance().elapsedTime.asSeconds() * 50) ,y + (Config::getInstance().elapsedTime.asSeconds() * 50));
	}
	return true;
}

void Enemy::circularShoot()
{
	double speed = 0.1;  
	double angle = 360 ; 

	double move_x = speed * cos( angle ) - sin(angle);
	double move_y = speed * sin( angle ) + cos(angle);

	this->sprite->move(move_x,move_y);
}

bool Enemy::circleTest(GameShape bullet)
{
	int spriteRadius = this->sprite->getRadius();
	int bulletRadius = bullet.getRadius();
	int xDistance = abs(this->sprite->getPosition().x-bullet.getPosition().x);
	int yDistance = abs(this->sprite->getPosition().y-bullet.getPosition().y);
	int distance = sqrt(xDistance*xDistance+yDistance*yDistance);
	if(distance<=spriteRadius+bulletRadius)
	{
		return true;
	}
	return false;
}