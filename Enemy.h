#pragma once
#include "Shooter.h"
#include <queue>
#include <memory>


/// <summary>
/// The player class represents the enemy
/// </summary>

class Enemy : public Shooter
{
public:
	Enemy(sf::RenderWindow& window, 
		std::queue<sf::Vector3f>* path,
		int type);

	~Enemy();


	void process();
	void Enemy::nextStep(int x0, int y0, int x1, int y1);

protected:
	void circularShoot();
	virtual int hitDetection();
	std::queue<sf::Vector3f>* path;
	sf::Vector3f currentPath;
	bool circleTest(GameShape& bullet);
};