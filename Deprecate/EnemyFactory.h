#pragma once
#include <SFML\Graphics\RenderWindow.hpp>
#include <list>
#include <map>


class Enemy;


/// <summary>
/// This is a factory which produces a set amount of bullets and then serves when requested. This class can also serve multiple bullets in a batch.
/// </summary>
class EnemyFactory{
public:

	EnemyFactory(sf::RenderWindow& window, int quantity, std::vector<Enemy*>& enemies);
	
	const enum EnemyType {standardShot = 1, heavyShot = 5};


	void returnObject(Enemy* enemy);
	std::vector<Enemy*> requestBatch(int quantity, EnemyFactory::EnemyType type);
	Enemy* requestObject(EnemyFactory::EnemyType type);

protected:
	void produceObjects(EnemyFactory::EnemyType type, int quantity);
	std::vector<Enemy*>* enemies;
	std::map<EnemyFactory::EnemyType , std::vector<Enemy*>> objects;
	sf::RenderWindow& window;
	int initQuantity;
};
