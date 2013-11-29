#include "Enemy.h"
#include "GameShape.h"
#include "BulletFactory.h"
#include "Bullet.h"
#include "Log.h"

Enemy::Enemy(sf::RenderWindow& window, 
			 std::queue<sf::Vector3f> path,
			 int type, int repeat,BulletFactory& bFactory,  
			 std::list<std::unique_ptr<Bullet>>& bullets,
			 std::unique_ptr<ResourceHandler>& resourceHandler,
			 const sf::Time& timeStep
			 ):
Shooter(window, bFactory, bullets, resourceHandler, timeStep)
{
	this->setType(Shooter::ShooterType::ENEMY);
	this->pathTemplate = path; // A qeueu which should not be touched (This is used to refill old queue
	this->enemyClock.restart();
	this->setRepeat(repeat);
	sprite = std::unique_ptr<GameShape>(new GameShape(GameShape::STARSHIP));

	setInitPath();
}

void Enemy::setInitPath()
{

	this->deleted = false;
	this->path = pathTemplate;
	this->currentPath = this->path.front();
	this->path.pop();

	this->sprite->setPosition(currentPath.x , currentPath.y);


}

Enemy::~Enemy(){
	LOGD("Deconstructor called for: Enemy#" << this);
}


void Enemy::shoot(int shoot)
{
	if(shoot != -1)
	{
		std::unique_ptr<Bullet> b = getBulletFactory().requestObject(Bullet::Type::standardShot);
		b->setOwner(this->getType());
		b->setPosition(this->sprite->getPosition().x , this->sprite->getPosition().y - 10);
		getBullets().push_back(std::move(b));
	}

}

void Enemy::process()
{
	this->shooterProcess();

	// Start
	sf::Vector2f length;
	length.x = abs(this->currentPath.x - path.front().x);
	length.y = abs(this->currentPath.y - path.front().y);

	// End
	sf::Vector2f currentPosition;
	currentPosition.x = abs(this->currentPath.x - this->sprite->getPosition().x);
	currentPosition.y = abs(this->currentPath.y - this->sprite->getPosition().y);



	float dx = this->path.front().x - this->currentPath.x;
	float dy = this->path.front().y - this->currentPath.y;
	float len = sqrtf(dx * dx + dy * dy);

	dx = (dx / len) * timeStep.asSeconds() * 50;
	dy = (dy / len) * timeStep.asSeconds() * 50;

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
		if(path.size() > 1)
		{
			path.pop();
		}
		else
		{
			LOGD("Enemy#" << this <<" delete flag set");
			this->deleted = true;
			if(this->getRepeat() == 1){
				setInitPath();
			}

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

int Enemy::getRepeat()
{
	return this->repeat;
}
void Enemy::setRepeat(int rep)
{
	this->repeat = rep;
}

