#include "Object.h"
#include "Log.h"
#include "GameShape.h"
#include <SFML\Graphics\RectangleShape.hpp>

void Object::draw()
{
	if(this->inited)
		this->window.draw(*this->sprite);
	
	sf::RectangleShape();
	sf::FloatRect bounds = this->sprite->getGlobalBounds();
	sf::RectangleShape af(sf::Vector2f(bounds.width,bounds.height));
	af.setPosition(bounds.left, bounds.top);
	af.setOutlineColor(sf::Color(141,23,22,23));
	af.setFillColor(sf::Color(255,255,255,150));
	
	this->window.draw(af);

}
void Object::process(){}

bool Object::getDeleted(){
	return this->deleted;
}

bool Object::getInited()
{
	return this->inited;
}
void Object::setInited(bool init)
{
	this->inited = init;
}

 Object::~Object()
 {

 }

// BulletFactory Getter/Setter
BulletFactory* Object::getBulletFactory()
{
	return this->bFactory;
}

void Object::setBulletFactory(BulletFactory* bFactorys)
{
	this->bFactory = bFactorys;
}

// Bullets getter/setter
std::vector<Bullet*>* Object::getBullets()
{
	return this->bullets;
}

void Object::setBullets(std::vector<Bullet*>& bullets)
{
	this->bullets = &bullets;
}

void Object::init(BulletFactory* bFactory, std::vector<Bullet*>& bullets)
{
	this->setBullets(bullets);
	this->setBulletFactory(bFactory);

	this->setInited(true);
}
