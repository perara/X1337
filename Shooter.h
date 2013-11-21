#pragma once
#include "Object.h"
#include "Bullet.h"
#include "BulletFactory.h"



class Shooter: public Object
{
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

};