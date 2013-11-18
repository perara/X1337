#include "BulletFactory.h"


/// <summary>
/// Initializes a new instance of the <see cref="BulletFactory"/> class.
/// </summary>
/// <param name="window">The render window.</param>
/// <param name="quantity">Input of the initial quantity the Factory should have at initialization.</param>
BulletFactory::BulletFactory(sf::RenderWindow& window, int quantity): 
	initQuantity(quantity), 
	window(window)
{
	produceObjects(1, quantity);  //TODO, implement TYPES
}

/// <summary>
/// Produces "quantity" amount of bullets of "type" x
/// </summary>
/// <param name="type">The type.</param>
/// <param name="quantity">The quantity.</param>
void BulletFactory::produceObjects(int type,int quantity){

	for(int i = 0; i < quantity; i++){
		this->objects[type].push_back(new Bullet(window, 1, this));
	}

}

/// <summary>
/// Requests a batch of bullets
/// </summary>
/// <param name="quantity">Quantity of bullets.</param>
/// <param name="type">Bullettype</param>
/// <returns>Returns a vector with the quantity of Bullet* requested</returns>
std::list<Bullet*> BulletFactory::requestBatch(int quantity, int type){
	std::list<Bullet*> retList;
	int i = 0;
	for (std::list<Bullet*>::iterator it = this->objects[type].begin(); it != this->objects[type].end() && i < quantity; it++ , i++){
		retList.push_back(*it);
	}
	return retList;
}


/// <summary>
/// Requests a single bullet*
/// </summary>
/// <param name="type">Bullet type</param>
/// <returns>Returns a single Bullet*</returns>
Bullet* BulletFactory::requestObject(int type){
	if(this->objects[type].size() < 50){
		this->produceObjects(type, this->initQuantity * 0.20); //Increase the size by 20%
	}

	Bullet* b = this->objects[type].front();
	this->objects[type].pop_front(); // O(0)
	//LOGD("DEBUG:: Bullet#" << b << " | Factory Size: " << this->objects[b->type].size());
	return  b;
}

void BulletFactory::returnObject(Bullet* bullet){
	//bullet->owner = new Object(window); // Set owner to dummy object
	this->objects[bullet->type].push_back(bullet); // O(0)
	//LOGD("DEBUG:: Bullet#" << bullet << " | Factory Size: " << this->objects[bullet->type].size());

}