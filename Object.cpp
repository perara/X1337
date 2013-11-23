#include "Object.h"
#include "Log.h"
#include "GameShape.h"

void Object::draw()
{
	if(this->inited)
		this->window.draw(*this->sprite);
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
std::list<Bullet*>* Object::getBullets()
{
	return this->bullets;
}

void Object::setBullets(std::list<Bullet*>& bullets)
{
	this->bullets = &bullets;
}

void Object::init(BulletFactory* bFactory, std::list<Bullet*>& bullets)
{
	this->setBullets(bullets);
	this->setBulletFactory(bFactory);

	this->setInited(true);
}
