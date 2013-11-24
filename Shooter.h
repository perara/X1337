#pragma once
#include "Object.h"

class BulletFactory;
class Bullet;

class Shooter: public Object
{
protected:

	Shooter(sf::RenderWindow& window
		);	
	int health;

	/* Health Related */
	void setHealth(int value);
	int getHealth();
	void decrementHealth();
	void incrementHealth();

	/* Processing */
	virtual void shooterProcess();

private:
	virtual int hitDetection() ;
	bool circleTest(GameShape& bullet);
	bool sat(GameShape* c1, GameShape* c2);

};