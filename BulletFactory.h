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

	BulletFactory(sf::RenderWindow& window, int quantity, std::vector<Bullet*>& bullets);

	void returnObject(Bullet* bullet);
	std::vector<Bullet*> requestBatch(int quantity, Bullet::Type type);
	Bullet* requestObject(Bullet::Type type);

protected:
	void produceObjects(Bullet::Type type, int quantity);
	std::vector<Bullet*>* bullets;
	std::map<Bullet::Type , std::vector<Bullet*>> objects;
	sf::RenderWindow& window;
	int initQuantity;
};
