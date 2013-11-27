#pragma once
#include <SFML\Graphics\RenderWindow.hpp>
#include <list>
#include <map>
#include "Bullet.h"

/// <summary>
/// This is a factory which produces a set amount of bullets and then serves when requested. This class can also serve multiple bullets in a batch.
/// </summary>
class BulletFactory{
public:

	BulletFactory(sf::RenderWindow& window, int quantity, std::list<Bullet*>& bullets);

	void returnObject(Bullet* bullet);
	std::list<Bullet*> requestBatch(int quantity, Bullet::Type type);
	Bullet* requestObject(Bullet::Type type);

protected:
	void produceObjects(Bullet::Type type, int quantity);
	std::list<Bullet*>& bullets;
	std::map<Bullet::Type , std::list<Bullet*>> objects;
	sf::RenderWindow& window;
	int initQuantity;
};
