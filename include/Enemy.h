#pragma once
#include "Shooter.h"
#include <queue>


/// <summary>
/// The player class represents the enemy
/// </summary>
class Enemy : public Shooter
{
public:
	const enum EnemyType
	{
		REGULAR = 1,
		CHUBBY = 2,
		BOSS = 3,
		DEATHSTAR = 4,

		POWERUP_HEALTH = -10,
		SHOOTERTYPECOUNT
	};

	Enemy(sf::RenderWindow& window,
		std::queue<sf::Vector3f> path,
		int type, int repeat,
		BulletFactory&,
		std::list<std::unique_ptr<Bullet>>&,
		std::shared_ptr<ResourceHandler>& resourceHandler,
		const sf::Time& timeStep);
	~Enemy();

	// Virtual draw function.
	virtual void draw();
	virtual void process();

	Enemy::EnemyType getEnemyType();

private:
	// Weither to repeat path ro not
	int repeat;
	int getRepeat();

	const std::queue<sf::Vector3f> pathTemplate; // Original Path (Which is the one loaded at start)
	std::queue<sf::Vector3f> path; // Path which is under progress
	void setInitPath();
	sf::Vector3f currentPath;

	// Clock for enemy shoot speed
	sf::Clock enemyClock;

	// Shoot function
	void shoot(int);

	// Variable for this enemy's type
	Enemy::EnemyType enemyType;
	void setEnemyType(Enemy::EnemyType);
};