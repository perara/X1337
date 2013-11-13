#pragma once
#include "Object.h"
#include "Bullet.h"
#include "BulletFactory.h"



class Shootable: public Object{
protected:
	std::list<std::shared_ptr<Bullet>>& bullets;
	BulletFactory* bFactory;

	Shootable(sf::RenderWindow& window, std::list<std::shared_ptr<Bullet>>& bullets, BulletFactory* bFactory);

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
	void deleteBullet(std::list<std::shared_ptr<Bullet>>::iterator& i);

	// TODO Hit detection
	sf::Vector2f GetSpriteCenter (const sf::CircleShape& Object);
	sf::Vector2f GetSpriteSize (const sf::CircleShape& Object);
	bool CircleTest(const sf::CircleShape& Object1, const sf::CircleShape& Object2);
};