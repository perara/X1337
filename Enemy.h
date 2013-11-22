#pragma once
#include "Shooter.h"
#include <queue>


/// <summary>
/// The player class represents the enemy
/// </summary>

class Enemy : public Shooter
{
public:
	Enemy::Enemy(sf::RenderWindow& window, 
		std::queue<sf::Vector3f>& path,
		int type
		);
	void process();
	void test();

protected:
	void circularShoot();
	virtual int hitDetection();
	std::queue<sf::Vector3f>& path;
	sf::Vector3f currentPath;
	bool circleTest(GameShape& bullet);
};