#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <list>
#include <map>
#include <memory>
#include "Bullet.h"

class ResourceManager;

/// <summary>
/// This is a factory which produces a set amount of bullets and then serves when requested. This class can serve multiple Bullet (s) in a batch.
/// </summary>
class BulletFactory{
public:

	BulletFactory(sf::RenderWindow& window,
		int quantity,
		const sf::Time& timeStep,
		std::shared_ptr<ResourceManager>& resourceHandler);

	// Returners
	void returnObject(std::unique_ptr<Bullet>& bullet);

	// Requesters
	std::list<std::unique_ptr<Bullet>> requestBatch(int quantity, Constants::BulletType type);
	std::unique_ptr<Bullet> requestObject(Constants::BulletType type);

private:
	// Initial quantity of the Bullet Factory
	int initQuantity;

	// Producers
	void produceObjects(Constants::BulletType type, int quantity);

	// Bullet FACTORY objects
	std::map<Constants::BulletType, std::list<std::unique_ptr<Bullet>>> objects;

	// Dependency injected
	sf::RenderWindow& window;
	const sf::Time& timeStep;
	std::shared_ptr<ResourceManager>& resourceHandler;
};
