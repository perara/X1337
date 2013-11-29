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
		REGULAR,
		BOSS,
		SHOOTERTYPECOUNT
	};

	Enemy(sf::RenderWindow& window, 
		std::queue<sf::Vector3f> path,
		int type, int repeat,
		BulletFactory&,   
		std::list<std::unique_ptr<Bullet>>&,
		std::unique_ptr<ResourceHandler>& resourceHandler,
		const sf::Time& timeStep);

	~Enemy();


	void process();
	int getRepeat();
	void setRepeat(int);
	Enemy::EnemyType getEnemyType();

protected:
	int repeat;
	void circularShoot();
	std::queue<sf::Vector3f> pathTemplate;
	std::queue<sf::Vector3f> path;
	sf::Vector3f currentPath;
	void setEnemyType(Enemy::EnemyType);
	
	
	sf::Clock enemyClock;
	void Enemy::shoot(int);

private:
	Enemy::EnemyType enemyType;
	void setInitPath();
};