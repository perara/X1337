#include "Enemy.h"
sf::Clock clsk;

Enemy::Enemy(sf::RenderWindow& window, 
			 sf::Vector2f startPos,
			 sf::Vector2f endPos,
			 float shootFrequency,
			 int radius, 
			 BulletFactory* bFactory, 
			 std::function<void(Object*)> sceneObjCallBack):
			 bFactory(bFactory),
			 sceneObjCallBack(sceneObjCallBack),
			 startPos(startPos),
			 endPos(endPos),
			 Object(window)
{
	this->sceneObjCallBack(this);
	this->sprite = new sf::CircleShape(radius,30);
	this->sprite->setPosition(startPos);



}

bool Enemy::process(){
	if((int)this->sprite->getPosition().x != (int)endPos.x || (int)this->sprite->getPosition().y != (int)endPos.y){
		float angle = atan2f(endPos.x - startPos.x, endPos.y - startPos.y) * 180 / 3.14;
		float x = sin(angle)*0.1f;
		float y = cos(angle)*0.1f;
		std::cout << this->sprite->getPosition().y << "-" << endPos.y << std::endl;
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


