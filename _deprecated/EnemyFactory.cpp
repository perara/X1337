#include "EnemyFactory.h"
#include "Enemy.h"

#include "Log.h"
/*
/// <summary>
/// Initializes a new instance of the <see cref="BulletFactory"/> class.
/// </summary>
/// <param name="window">The render window.</param>
/// <param name="quantity">Input of the initial quantity the Factory should have at initialization.</param>
EnemyFactory::EnemyFactory(sf::RenderWindow& window, int quantity, std::vector<Enemy*>& enemies): 
	initQuantity(quantity), 
	window(window),
	enemies(&enemies)
{

	produceObjects(EnemyType::heavyShot, quantity);  //TODO, implement TYPES
	produceObjects(EnemyType::standardShot, quantity);

}
/// <summary>
/// Produces "quantity" amount of bullets of "type" x
/// </summary>
/// <param name="type">The type.</param>
/// <param name="quantity">The quantity.</param>
void EnemyFactory::produceObjects(EnemyFactory::EnemyType type,int quantity)
{
	for(int i = 0; i < quantity; i++)
	{
		Enemy* e1 = new Enemy(
			window, 
			nullptr,
			type);

		
		//b->init(this, *this->bullets);

		this->objects[type].push_back(b);

	}
}
/// <summary>
/// Requests a batch of bullets
/// </summary>
/// <param name="quantity">Quantity of bullets.</param>
/// <param name="type">Bullettype</param>
/// <returns>Returns a vector with the quantity of Bullet* requested</returns>
std::vector<Enemy*> EnemyFactory::requestBatch(int quantity, EnemyFactory::EnemyType type)
{
	std::vector<Enemy*> retList;
	int i = 0;
	for (std::vector<Enemy*>::iterator it = this->objects[type].begin(); it != this->objects[type].end() && i < quantity; it++ , i++)
	{
		retList.push_back(*it);
	}
	return retList;
}
/// <summary>
/// Requests a single bullet*
/// </summary>
/// <param name="type">Bullet type</param>
/// <returns>Returns a single Bullet*</returns>
Enemy* EnemyFactory::requestObject(EnemyFactory::EnemyType type)
{
	if(this->objects[type].size() < 50)
	{
		this->produceObjects(type, this->initQuantity * 0.20); //Increase the size by 20%
	}

	Enemy* b = this->objects[type].back();
	this->objects[type].pop_back(); // O(0)
	//LOGD("DEBUG:: Bullet#" << b << " | Factory Size: " << this->objects[type].size());
	return  b;
}
void EnemyFactory::returnObject(Enemy* bullet)
{
	bullet->resetObject();
	this->objects[bullet->getBulletType()].push_back(bullet); // O(0)

	//std::cout << "DEBUG:: Bullet#" << bullet << " | Factory Size: " << this->objects[bullet->getBulletType()].size() << std::endl;
	LOGD("DEBUG:: Bullet#" << bullet << " | Factory Size: " << this->objects[bullet->getBulletType()].size());
}*/