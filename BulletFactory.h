#pragma once
#include "Bullet.h"
#include <vector>
#include <map>

class BulletFactory{
public:
	BulletFactory();
	BulletFactory(sf::RenderWindow& window, int quantity);
	void produceObjects(int type, int quantity);
	std::vector<Bullet*> requestBatch(int quantity, int type);
	Bullet* requestObject(int type);

protected:
	std::map<int, std::vector<Bullet*>> objects;
	sf::RenderWindow& window;
};
