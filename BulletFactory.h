#pragma once
#include <SFML\Graphics\RenderWindow.hpp>
#include <list>
#include <map>


class Bullet;


/// <summary>
/// This is a factory which produces a set amount of bullets and then serves when requested. This class can also serve multiple bullets in a batch.
/// </summary>
class BulletFactory{
public:

	BulletFactory(sf::RenderWindow& window, int quantity, std::vector<Bullet*>& bullets);
	
	const enum BulletType {standardShot = 1, heavyShot = 5};


	void returnObject(Bullet* bullet);
	std::vector<Bullet*> requestBatch(int quantity, BulletFactory::BulletType type);
	Bullet* requestObject(BulletFactory::BulletType type);

protected:
	void produceObjects(BulletFactory::BulletType type, int quantity);
	std::vector<Bullet*>* bullets;
	std::map<BulletFactory::BulletType , std::vector<Bullet*>> objects;
	sf::RenderWindow& window;
	int initQuantity;
};
