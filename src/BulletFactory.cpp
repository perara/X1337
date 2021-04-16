#include "../include/BulletFactory.h"
#include <memory>
#include <cmath>

/// <summary>
/// Initializes a new instance of the <see cref="BulletFactory"/> class.
/// </summary>
/// <param name="window">The window.</param>
/// <param name="quantity">The quantity.</param>
/// <param name="bullets">The bullets.</param>
/// <param name="timeStep">The time step.</param>
/// <param name="resourceHandler">The resource handler.</param>
BulletFactory::BulletFactory(sf::RenderWindow& window,
							 int quantity,
							 const sf::Time& timeStep,
							 std::shared_ptr<ResourceManager>& resourceHandler) :
							 initQuantity(quantity),
							 window(window),
	timeStep(timeStep),
	resourceHandler(resourceHandler)
{

	// Produce object of each of the bullets
	produceObjects(Constants::BulletType::standardShot, quantity);
	produceObjects(Constants::BulletType::heavyShot, quantity);

}
/// <summary>
/// Produces "quantity" amount of bullets of "type" x
/// </summary>
/// <param name="type">The type.</param>
/// <param name="quantity">The quantity.</param>
void BulletFactory::produceObjects(Constants::BulletType type, int quantity)
{

	// Produce a set amount (quantity) of bullets,
	for (int i = 0; i < quantity; i++)
	{
		// Create bullet
		std::unique_ptr<Bullet>  b = std::make_unique<Bullet>(window, type, timeStep, resourceHandler);
		this->objects[type].push_back(std::move(b));

	}
}
/// <summary>
/// Requests a batch of bullets
/// </summary>
/// <param name="quantity">Quantity of bullets.</param>
/// <param name="type">Bullettype</param>
/// <returns>Returns a vector with the quantity of Bullet* requested</returns>
std::list<std::unique_ptr<Bullet>> BulletFactory::requestBatch(int quantity, Constants::BulletType type)
{
	// Checks if there is less than "quantity" bullets left in the factory
	if (objects[type].size() < quantity)
	{
		produceObjects(type, std::ceil(initQuantity * 0.20)); //Increase the size by 20%
	}

	// Create a return list 
	std::list<std::unique_ptr<Bullet>> retList;

	// Loop through the "quantity" number of bullets requested.
	for (int i = 0; i < quantity; i++)
	{
		// Get bullets and pop from the list
		std::unique_ptr<Bullet> b = std::move(objects[type].back());
		objects[type].pop_back(); // O(0)

		// Push to the return list
		retList.push_back(std::move(b));
	}

	return retList;
}

/// <summary>
/// Requests a single bullet*
/// </summary>
/// <param name="type">Bullet type</param>
/// <returns>Returns a single Bullet*</returns>
std::unique_ptr<Bullet> BulletFactory::requestObject(Constants::BulletType type)
{
	// Checks if there is less than 50 bullets left in the factory
	if (objects[type].size() < 50)
	{
		produceObjects(type, std::ceil(initQuantity * 0.20)); //Increase the size by 20%
	}

	// Get a bullet
	std::unique_ptr<Bullet> b = std::move(objects[type].back());
	objects[type].pop_back(); // O(0)
	//LOGD("DEBUG:: Bullet#" << b << " | Factory Size: " << this->objects[type].size());
	return  b;
}

/// <summary>
/// Returns the bullet to the bullet factory
/// </summary>
/// <param name="bullet">The bullet.</param>
void BulletFactory::returnObject(std::unique_ptr<Bullet>& bullet)
{
	// Return a bullet
	//LOGD("DEBUG:: Bullet#" << bullet << " | Factory Size: " << this->objects[bullet->getBulletType()].size());
	bullet->resetObject();
	this->objects[bullet->getBulletType()].push_back(std::move(bullet)); // O(0)
}