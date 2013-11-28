#include "BulletFactory.h"
#include "Log.h"


/// <summary>
/// Initializes a new instance of the <see cref="BulletFactory"/> class.
/// </summary>
/// <param name="window">The render window.</param>
/// <param name="quantity">Input of the initial quantity the Factory should have at initialization.</param>
BulletFactory::BulletFactory(sf::RenderWindow& window, int quantity, std::list<Bullet*>& bullets): 
	initQuantity(quantity), 
	window(window),
	bullets(bullets)
{

	produceObjects(Bullet::Type::standardShot, quantity);  //TODO, implement TYPES
	produceObjects(Bullet::Type::heavyShot, quantity);

}
/// <summary>
/// Produces "quantity" amount of bullets of "type" x
/// </summary>
/// <param name="type">The type.</param>
/// <param name="quantity">The quantity.</param>
void BulletFactory::produceObjects(Bullet::Type type,int quantity)
{
	for(int i = 0; i < quantity; i++)
	{
		Bullet*  b = new Bullet(window, type);
		b->init();

		this->objects[type].push_back(b);

	}
}
/// <summary>
/// Requests a batch of bullets
/// </summary>
/// <param name="quantity">Quantity of bullets.</param>
/// <param name="type">Bullettype</param>
/// <returns>Returns a vector with the quantity of Bullet* requested</returns>
std::list<Bullet*> BulletFactory::requestBatch(int quantity, Bullet::Type type)
{
	std::list<Bullet*> retList;
	int i = 0;
	for (std::list<Bullet*>::iterator it = this->objects[type].begin(); it != this->objects[type].end() && i < quantity; it++ , i++)
	{
		retList.push_back(*it); // TODO might not work?
	}
	return retList;
}
/// <summary>
/// Requests a single bullet*
/// </summary>
/// <param name="type">Bullet type</param>
/// <returns>Returns a single Bullet*</returns>
Bullet* BulletFactory::requestObject(Bullet::Type type)
{
	if(this->objects[type].size() < 50)
	{
		this->produceObjects(type, this->initQuantity * 0.20); //Increase the size by 20%
	}

	Bullet* b = this->objects[type].back();
	this->objects[type].pop_back(); // O(0)
	//LOGD("DEBUG:: Bullet#" << b << " | Factory Size: " << this->objects[type].size());
	return  b;
}
void BulletFactory::returnObject(Bullet* bullet)
{
	bullet->resetObject();
	this->objects[bullet->getBulletType()].push_back(bullet); // O(0)

	//std::cout << "DEBUG:: Bullet#" << bullet << " | Factory Size: " << this->objects[bullet->getBulletType()].size() << std::endl;
	LOGD("DEBUG:: Bullet#" << bullet << " | Factory Size: " << this->objects[bullet->getBulletType()].size());
}