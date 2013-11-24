#include "Enemy.h"
#include "GameShape.h"
#include "BulletFactory.h"
#include "Bullet.h"
#include "Log.h"
#include "Globals.h"

Enemy::Enemy(sf::RenderWindow& window, 
			 std::queue<sf::Vector3f>* path,
			 int type
			 ):
Shooter(window)
{
	this->enemyClock.restart();
	this->path = path;
	this->currentPath = this->path->front();
	this->path->pop();

	this->setType(Shooter::ShooterType::REGULAR);
	this->sprite = new GameShape(GameShape::STARSHIP);
	this->sprite->setPosition(currentPath.x , currentPath.y);

}

Enemy::~Enemy(){
	LOGD("Deconstructor called for: Enemy#" << this);

	delete this->path;
}


void Enemy::shoot(int shoot)
{
	if(shoot != -1)
	{
		Bullet* b = this->getBulletFactory()->requestObject(BulletFactory::BulletType::standardShot);
		b->setOwner(this->getType());
		b->setPosition(this->sprite->getPosition().x , this->sprite->getPosition().y - 10);
		this->getBullets()->push_back(b);
	}

}

void Enemy::process()
{
	if(!this->getInited()) return;

	this->shooterProcess();

	// Start
	sf::Vector2f length;
	length.x = abs(this->currentPath.x - path->front().x);
	length.y = abs(this->currentPath.y - path->front().y);

	// End
	sf::Vector2f currentPosition;
	currentPosition.x = abs(this->currentPath.x - this->sprite->getPosition().x);
	currentPosition.y = abs(this->currentPath.y - this->sprite->getPosition().y);



	float dx = this->path->front().x - this->currentPath.x;
	float dy = this->path->front().y - this->currentPath.y;
	float len = sqrtf(dx * dx + dy * dy);

	dx = (dx / len) * Globals::getInstance().getTimeStep().asSeconds() * 50;
	dy = (dy / len) * Globals::getInstance().getTimeStep().asSeconds() * 50;

	if(this->enemyClock.getElapsedTime().asMilliseconds() > 400)
	{
		this->shoot(this->currentPath.z);
		this->enemyClock.restart();
	}

	if(
		currentPosition.x < length.x || 
		currentPosition.y < length.y
		)
	{

		if(currentPosition.x < length.x)
		{
			this->sprite->move(dx ,0 );
		}

		if(currentPosition.y < length.y)
		{
			this->sprite->move(0 ,dy );
		}

	}
	else
	{
		//std::cout << currentPath.x << ","<< currentPath.y << "->" << this->path->front().x << "," << this->path->front().y << std::endl;
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


void Enemy::circularShoot()
{
	double speed = 0.1;  
	double angle = 360 ; 

	double move_x = speed * cos( angle ) - sin(angle);
	double move_y = speed * sin( angle ) + cos(angle);

	this->sprite->move(move_x,move_y);
}
