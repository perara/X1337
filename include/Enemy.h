#pragma once
#include "Shooter.h"
#include <queue>
#include "VectorN.h"

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
		std::queue<VectorN> path,
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
	// Weither to repeat path or not
	int repeat;
	int getRepeat();

	// Acceleration and speed
	float acceleration;
	sf::Vector2f speed;

	// The path and path template
	const std::queue<VectorN> pathTemplate; // Original Path (Which is the one loaded at start)
	std::queue<VectorN> path; // Path which is under progress

	// The emote queue, and the 
	std::list<std::pair<int, std::string>> emoteQueue;


	void setInitPath();
	VectorN currentPath;

	// Clock for enemy shoot speed
	sf::Clock enemyClock;

	sf::Time shootTime; // Time which indicates weither to shoot or not
	sf::Time sleepTime; // Time which indicates weither to sleep or not

	// Shoot function
	void shoot(int);

	// Processes emotes done by the enemy
	void emotes();

	// Movement handler for the enemy
	void movement();

	// A function which processes the underlying shooting processing (This calls shoot())
	void shootProcess();

	// Variable for this enemy's type
	Enemy::EnemyType enemyType;
	void setEnemyType(Enemy::EnemyType);

	int secondRot;
};