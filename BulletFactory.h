#pragma once
#include "Bullet.h"


#include <list>

/// <summary>
/// This is a factory which produces a set amount of bullets and then serves when requested. This class can also serve multiple bullets in a batch.
/// </summary>
class BulletFactory{
public:
	BulletFactory();
	BulletFactory(sf::RenderWindow& window, int quantity);
	
	void returnObject(Bullet* bullet);
	std::list<Bullet*> requestBatch(int quantity, int type);
	Bullet* requestObject(int type);

protected:
	void produceObjects(int type, int quantity);

	std::map<int, std::list<Bullet*>> objects;
	sf::RenderWindow& window;
	int initQuantity;
};
