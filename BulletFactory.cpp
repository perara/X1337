#include "BulletFactory.h"
#include <iostream>
BulletFactory::BulletFactory(sf::RenderWindow& window, int quantity): window(window){
	produceObjects(1, quantity);
}
void BulletFactory::produceObjects(int type,int quantity){

	for(int i = 0; i < quantity; i++){
		this->objects[type].push_back(new Bullet(this->window, 1));
	}

}
std::vector<Bullet*> BulletFactory::requestBatch(int quantity, int type){
	std::vector<Bullet*>::const_iterator first = this->objects[type].begin() + (this->objects[type].size() - quantity);
	std::vector<Bullet*>::const_iterator last = this->objects[type].begin() + this->objects[type].size();
	std::vector<Bullet*> newVec(first, last);
	return newVec;
}

Bullet* BulletFactory::requestObject(int type){
	Bullet* b = this->objects[type].back();
	this->objects[type].pop_back();
	std::cout << "DEBUG:: Bullet#" << b << " | Factory Size: " << this->objects.size() << std::endl;
	return  b;
}
