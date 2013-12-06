#pragma once
#include <SFML\Graphics\RenderWindow.hpp>
#include <list>
#include <map>
#include <memory>
#include "Bullet.h"

/// <summary>
/// This is a factory which produces a set amount of bullets and then serves when requested. This class can also serve multiple bullets in a batch.
/// </summary>
class BulletFactory{
public:

	BulletFactory(sf::RenderWindow& window, int quantity, std::list<std::unique_ptr<Bullet>>& bullets, const sf::Time& timeStep);

	void returnObject(std::unique_ptr<Bullet> bullet);
	std::list<std::unique_ptr<Bullet>> requestBatch(int quantity, Bullet::Type type);
	std::unique_ptr<Bullet> requestObject(Bullet::Type type);

private:
	void produceObjects(Bullet::Type type, int quantity);
	std::list<std::unique_ptr<Bullet>>& bullets;
	std::map<Bullet::Type, std::list<std::unique_ptr<Bullet>>> objects;
	sf::RenderWindow& window;
	int initQuantity;
	const sf::Time& timeStep;
};
