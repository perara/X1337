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

protected:
	void circularShoot();
	std::queue<sf::Vector3f>* path;
	sf::Vector3f currentPath;

	sf::Clock enemyClock;
	void Enemy::shoot(int);
};