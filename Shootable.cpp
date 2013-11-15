#include "Shootable.h"

Shootable::Shootable(sf::RenderWindow& window, std::list<Bullet*>& bullets, BulletFactory* bFactory): 
	Object(window),
	bullets(bullets),
	bFactory(bFactory)
{health = 100;}

int Shootable::hitDetection(){
	// COLLISION TODO
	int hitCounter = 0;

	for(auto&it : bullets){
		bool upForDeletion = it->process();
		bool wasHit = wasHit = CircleTest(*it->sprite,  *this->sprite);

		if(upForDeletion){
			it->deleteQueued = true;

		}else if(wasHit && this != it->owner){
			//this->deleteBullet(i);
			it->deleteQueued = true;
			hitCounter++;
		}
	}

	

	return hitCounter;
}

void Shootable::setHealth(int value){
	this->health = value;
}

int Shootable::getHealth(){
	return this->health;
}

void Shootable::decrementHealth(){
	this->health--;
}

void Shootable::incrementHealth(){
	this->health++;
}

void Shootable::shootableProcess(){
	int hitCount = this->hitDetection();
	
	if(hitCount > 0){
		LOGD("Shootable#"<<this << " has " << this->getHealth() << " health");
		this->decrementHealth();
	}
}









/* Privates */
sf::Vector2f Shootable::GetSpriteCenter (const sf::CircleShape& Object)
{
	sf::FloatRect AABB = Object.getGlobalBounds();
	return sf::Vector2f (AABB.left+AABB.width/2.f, AABB.top+AABB.height/2.f);
}

sf::Vector2f Shootable::GetSpriteSize (const sf::CircleShape& Object)
{
	sf::IntRect OriginalSize = Object.getTextureRect();
	sf::Vector2f Scale = Object.getScale();
	return sf::Vector2f (OriginalSize.width*Scale.x, OriginalSize.height*Scale.y);
}

bool Shootable::CircleTest(const sf::CircleShape& Object1, const sf::CircleShape& Object2) {
	sf::Vector2f Obj1Size = GetSpriteSize(Object1);
	sf::Vector2f Obj2Size = GetSpriteSize(Object2);
	float Radius1 = Object1.getRadius();
	float Radius2 = Object2.getRadius();

	sf::Vector2f Distance = GetSpriteCenter(Object1)-GetSpriteCenter(Object2);

	return (Distance.x * Distance.x + Distance.y * Distance.y <= (Radius1 + Radius2) * (Radius1 + Radius2));
}

void Shootable::deleteBullet(std::list<Bullet*>::iterator& i){
	this->bFactory->returnObject( (Bullet*)*i); // TODO CLEAN?
	bullets.erase(i--); 
}