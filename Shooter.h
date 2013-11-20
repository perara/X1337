#pragma once
#include "Object.h"
#include "Bullet.h"
#include "BulletFactory.h"



class Shooter: public Object{
protected:
	std::list<Bullet*>& bullets;
	BulletFactory* bFactory;
	Shooter(sf::RenderWindow& window, std::list<Bullet*>& bullets, BulletFactory* bFactory);

	/* Health Related */
	void setHealth(int value);
	int getHealth();
	void decrementHealth();
	void incrementHealth();

	/* Processing */
	virtual void shootableProcess();

private:
	int health;
	virtual int hitDetection();

	// TODO Hit detection
	sf::Vector2f GetSpriteCenter (const sf::CircleShape& Object);
	sf::Vector2f GetSpriteSize (const sf::CircleShape& Object);
	bool CircleTest(const sf::CircleShape& Object1, const sf::CircleShape& Object2);
};