#include "Enemy.h"
#include "GameShape.h"
#include "BulletFactory.h"
#include "Bullet.h"

//#include "stdlib.h"
sf::Clock clsk;

Enemy::Enemy(sf::RenderWindow& window, 
			 std::queue<sf::Vector3f>& path,
			 int type
			 ):
	path(path),
	Shooter(window)
{
	currentPath = path.front();
	path.pop();

	this->sprite = new GameShape(GameShape::circle, 10);
	this->sprite->setPosition(currentPath.x , path.front().y);
	

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
	/*std::cout << currentPath.x << "," << currentPath.y << std::endl;
	std::cout << path.size() << std::endl;
	if(
		(int)this->sprite->getPosition().x != (int)path.front().x && 
		(int)this->sprite->getPosition().y != (int)path.front().y)
	{

		float angle = atan2f(path.front().x - currentPath.x, path.front().y - currentPath.y) * 180 / 3.14;
		float x = sin(angle) * (Globals::getInstance().getTimeStep().asSeconds() * 100);
		float y = cos(angle) * (Globals::getInstance().getTimeStep().asSeconds() * 100);
		this->sprite->move(x ,y );
	}else
	{
		this->deleted = true;
	}
	*/
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