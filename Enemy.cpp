#include "Enemy.h"
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
	this->sprite = new sf::CircleShape(radius,30);
	this->sprite->setPosition(startPos);



}

bool Enemy::process(){
	this->shootableProcess();


	if((int)this->sprite->getPosition().x != (int)endPos.x || (int)this->sprite->getPosition().y != (int)endPos.y){
		float angle = atan2f(endPos.x - startPos.x, endPos.y - startPos.y) * 180 / 3.14;
		float x = sin(angle)*0.1f;
		float y = cos(angle)*0.1f;
		this->sprite->move(x,y);

	}
	return true;
}



void Enemy::circularShoot(){
	double speed = 0.1;  
	double angle = 360 ; 

	double move_x = speed * cos( angle ) - sin(angle);
	double move_y = speed * sin( angle ) + cos(angle);


	this->sprite->move(move_x,move_y);

}


