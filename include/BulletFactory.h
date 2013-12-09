#pragma once
#include <SFML\Graphics\RenderWindow.hpp>
#include <list>
#include <map>
#include <memory>
#include "Bullet.h"

class ResourceHandler;

/// <summary>
/// This is a factory which produces a set amount of bullets and then serves when requested. This class can serve multiple Bullet (s) in a batch.
/// </summary>
class BulletFactory{
public:

	BulletFactory(sf::RenderWindow& window,
		int quantity,
		std::list<std::unique_ptr<Bullet>>& bullets,
		const sf::Time& timeStep,
		std::shared_ptr<ResourceHandler>& resourceHandler);

	// Returners
	void returnObject(std::unique_ptr<Bullet>& bullet);

	// Requesters
	std::list<std::unique_ptr<Bullet>> requestBatch(int quantity, Bullet::Type type);
	std::unique_ptr<Bullet> requestObject(Bullet::Type type);

private:
	// Initial quantity of the Bullet Factory
	int initQuantity;

	// Producers
	void produceObjects(Bullet::Type type, int quantity);

	// Bullet list from World.cpp
	std::list<std::unique_ptr<Bullet>>& bullets;

	// Bullet FACTORY objects
	std::map<Bullet::Type, std::list<std::unique_ptr<Bullet>>> objects;

	// Dependency injected
	sf::RenderWindow& window;
	const sf::Time& timeStep;
	std::shared_ptr<ResourceHandler>& resourceHandler;
};
