#include "BulletFactory.h"
#include "Log.h"
#include "ResourceHandler.h";


/// <summary>
/// Initializes a new instance of the <see cref="BulletFactory"/> class.
/// </summary>
/// <param name="window">The render window.</param>
/// <param name="quantity">Input of the initial quantity the Factory should have at initialization.</param>
BulletFactory::BulletFactory(sf::RenderWindow& window,
	int quantity,
	std::list<std::unique_ptr<Bullet>>& bullets,
	const sf::Time& timeStep,
	std::shared_ptr<ResourceHandler>& resourceHandler) :
	initQuantity(quantity),
	window(window),
	bullets(bullets),
	timeStep(timeStep),
	resourceHandler(resourceHandler)
{

	produceObjects(Bullet::Type::standardShot, quantity);  //TODO, implement TYPES
	produceObjects(Bullet::Type::heavyShot, quantity);

}
/// <summary>
/// Produces "quantity" amount of bullets of "type" x
/// </summary>
/// <param name="type">The type.</param>
/// <param name="quantity">The quantity.</param>
void BulletFactory::produceObjects(Bullet::Type type, int quantity)
{
	for (int i = 0; i < quantity; i++)
	{
		std::unique_ptr<Bullet>  b = std::unique_ptr<Bullet>(new Bullet(window, type, timeStep, resourceHandler));

		this->objects[type].push_back(std::move(b));

	}
}
/// <summary>
/// Requests a batch of bullets
/// </summary>
/// <param name="quantity">Quantity of bullets.</param>
/// <param name="type">Bullettype</param>
/// <returns>Returns a vector with the quantity of Bullet* requested</returns>
std::list<std::unique_ptr<Bullet>> BulletFactory::requestBatch(int quantity, Bullet::Type type)
{
	std::list<std::unique_ptr<Bullet>> retList;
	for (int i = 0; i < quantity; i++)
	{
		std::unique_ptr<Bullet> b = std::move(objects[type].back());
		objects[type].pop_back(); // O(0)
		retList.push_back(std::move(b)); // TODO might not work?
	}
	return retList;
}
/// <summary>
/// Requests a single bullet*
/// </summary>
/// <param name="type">Bullet type</param>
/// <returns>Returns a single Bullet*</returns>
std::unique_ptr<Bullet> BulletFactory::requestObject(Bullet::Type type)
{
	if (objects[type].size() < 50)
	{
		produceObjects(type, initQuantity * 0.20); //Increase the size by 20%
	}

	std::unique_ptr<Bullet> b = std::move(objects[type].back());
	objects[type].pop_back(); // O(0)
	//LOGD("DEBUG:: Bullet#" << b << " | Factory Size: " << this->objects[type].size());
	return  b;
}
void BulletFactory::returnObject(std::unique_ptr<Bullet> bullet)
{
	LOGD("DEBUG:: Bullet#" << bullet << " | Factory Size: " << this->objects[bullet->getBulletType()].size());
	bullet->resetObject();
	this->objects[bullet->getBulletType()].push_back(std::move(bullet)); // O(0)
}