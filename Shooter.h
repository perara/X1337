#pragma once
#include "Object.h"

class BulletFactory;
class Bullet;

class Shooter: public Object
{
public:
	const enum ShooterType
	{
		REGULAR,
		BOSS,
		PLAYER,
		SHOOTERTYPECOUNT
	};

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
	void setType(Shooter::ShooterType);
	ShooterType& getType();


private:
	virtual void hitDetection() ;
	bool circleTest(GameShape& bullet);
	bool sat(GameShape* c1, GameShape* c2);
	
	ShooterType shooterType;
};