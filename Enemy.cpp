#include "Enemy.h"
#include "GameShape.h"
#include "BulletFactory.h"
#include "Bullet.h"
#include "Log.h"
#include "Globals.h"
#include <math.h>       /* cos */

#define PI 3.14159265

//#include "stdlib.h"
sf::Clock clsk;

Enemy::Enemy(sf::RenderWindow& window, 
			 std::queue<sf::Vector3f>* path,
			 int type
			 ):
Shooter(window)
{
	this->path = path;
	this->currentPath = this->path->front();
	this->path->pop();

	this->sprite = new GameShape(GameShape::circle, 10);
	this->sprite->setPosition(currentPath.x , currentPath.y);

}

Enemy::~Enemy(){
	LOGD("Deconstructor called for: Enemy#" << this);

	delete this->path;
}




int Enemy::hitDetection()
{
	// COLLISION TODO
	int hitCounter = 0;


	if(!this->getBullets()->empty())
	{
		for(auto& i: *this->getBullets())
		{
			bool wasHit = false;
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

void Enemy::process()
{
	if(!this->getInited()) return;

	this->shootableProcess();


	sf::Vector2f length;
	length.x = abs(this->currentPath.x - path->front().x);
	length.y = abs(this->currentPath.y - path->front().y);

	sf::Vector2f currentPosition;
	currentPosition.x = abs(this->currentPath.x - this->sprite->getPosition().x);
	currentPosition.y = abs(this->currentPath.y - this->sprite->getPosition().y);



	float dx = this->path->front().x - this->currentPath.x;
	float dy = this->path->front().y - this->currentPath.y;
	float len = sqrtf(dx * dx + dy * dy);
	dx = (dx / len) * Globals::getInstance().getTimeStep().asSeconds() * 100;
	dy = (dy / len) * Globals::getInstance().getTimeStep().asSeconds() * 100;

	if(
		currentPosition.x <= length.x || 
		currentPosition.y <= length.y &&
		length.x != 0 &&
		length.y != 0
		)
	{

		if(currentPosition.x <= length.x)
		{
			this->sprite->move(dx ,0 );
		}

		if(currentPosition.y <= length.y)
		{
			this->sprite->move(0 ,dy );
		}

	}
	else
	{
		currentPath = path->front();
		if(path->size() > 1)
		{
			path->pop();
		}
		else
		{
			LOGD("Enemy#" << this <<" delete flag set");
			this->deleted = true;
		}

	}



}

void Enemy::nextStep(int x0, int y0, int x1, int y1)
{
	float slope = ((float)y1 - y0)/ ((float) x1 - x0);
	float err = 0.0;
	int y = 0;

	this->sprite->move(x0,y);
	err += slope;
	if(err >= 0.5)
	{
		y++;
		err -= 1.0;
	}




}


void Enemy::circularShoot()
{
	double speed = 0.1;  
	double angle = 360 ; 

	double move_x = speed * cos( angle ) - sin(angle);
	double move_y = speed * sin( angle ) + cos(angle);

	this->sprite->move(move_x,move_y);
}

bool Enemy::circleTest(GameShape& bullet)
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