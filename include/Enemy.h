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


	Enemy(sf::RenderWindow& window,
		std::queue<VectorN> path,
		std::list<std::pair<int, std::string>> emoteQueue,
		int type, int repeat,
		BulletFactory&,
		std::list<std::unique_ptr<Bullet>>&,
		std::shared_ptr<ResourceManager>& resourceHandler,
		const sf::Time& timeStep);
	~Enemy();

	// Virtual draw function.
	virtual void draw();
	virtual void process();

	Constants::EnemyC::Type getEnemyType();

private:
	// Weither to repeat path or not
	int repeat;
	int getRepeat() const;

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
    Constants::EnemyC::Type enemyType;
	void setEnemyType(Constants::EnemyC::Type);

	float secondRot;
};