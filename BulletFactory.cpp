#include "BulletFactory.h"
#include <iostream>


BulletFactory::BulletFactory(sf::RenderWindow& window, int quantity): initQuantity(quantity), window(window){
	produceObjects(1, quantity);
}
void BulletFactory::produceObjects(int type,int quantity){

	for(int i = 0; i < quantity; i++){
		this->objects[type].push_back(new Bullet(window, 1));
	}

}
std::vector<Bullet*> BulletFactory::requestBatch(int quantity, int type){
	std::vector<Bullet*>::const_iterator first = this->objects[type].begin() + (this->objects[type].size() - quantity);
	std::vector<Bullet*>::const_iterator last = this->objects[type].begin() + this->objects[type].size();
	std::vector<Bullet*> newVec(first, last);
	return newVec;

}

Bullet* BulletFactory::requestObject(int type){
	if(this->objects[type].size() < 50){
		this->produceObjects(type, this->initQuantity * 0.20); //Increase the size by 20%
	}

	Bullet* b = this->objects[type].back();
	this->objects[type].pop_back();
	//LOGD("DEBUG:: Bullet#" << b << " | Factory Size: " << this->objects[b->type].size());
	return  b;
}

void BulletFactory::returnObject(Bullet* bullet){
	this->objects[bullet->type].push_back(bullet);
	//LOGD("DEBUG:: Bullet#" << bullet << " | Factory Size: " << this->objects[bullet->type].size());

}