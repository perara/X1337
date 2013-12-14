#pragma once
#include "Shooter.h"
#include <queue>


/// <summary>
/// Enemy is represented as a shooter. Each of the Enemy object is a unique enemy.
/// </summary>
class Enemy : public Shooter
{
public:

	/// Enumerable which contains all EnemyType 's implemented into the game.
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
		std::list<std::pair<int, std::string>> emoteQueue,
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

	// The path and path template
	const std::queue<sf::Vector3f> pathTemplate; // Original Path (Which is the one loaded at start)
	std::queue<sf::Vector3f> path; // Path which is under progress

	// The emote queue, and the 
	std::list<std::pair<int, std::string>> emoteQueue;


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