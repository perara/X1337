#pragma once
#include "Shootable.h"

/// <summary>
/// The player class represents the enemy
/// </summary>
class Enemy : public Shootable{
	sf::Vector2f startPos;
	sf::Vector2f endPos;

public:
	Enemy::Enemy(sf::RenderWindow& window, 
		sf::Vector2f startPos,
		sf::Vector2f endPos,
		float shootFrequency,
		int radius, 
		BulletFactory* bFactory, 
		std::list<Bullet*>& bullets);
	bool process();
	void test();

protected:
	void circularShoot();
	
};