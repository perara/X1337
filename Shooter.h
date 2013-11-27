#pragma once
#include "Object.h"

class BulletFactory;
class Bullet;

class Shooter: public Object
{
public:

	const enum ShooterType
	{
		PLAYER,
		ENEMY
	};

	virtual void init(BulletFactory&, std::list<Bullet*>&);
	int getHealth(); // Todo should be protected
	Shooter::ShooterType getType();
protected:

	Shooter(sf::RenderWindow& window
		);	
	int health;

	/* Health Related */
	void setHealth(int value);

	void decrementHealth();
	void incrementHealth();

	/* Processing */
	virtual void shooterProcess();

	void setType(Shooter::ShooterType);


	// Bullets getter/setter
	std::list<Bullet*>* getBullets();
	void setBullets(std::list<Bullet*>& bullets);

	// BulletFactory Getter/Setter
	BulletFactory* getBulletFactory();
	void setBulletFactory(BulletFactory& bFactory);




private:
	virtual void hitDetection() ;
	bool circleTest(GameShape& bullet);
	bool sat(GameShape* c1, GameShape* c2);
	std::list<Bullet*>* bullets;
	ShooterType shooterType;

	BulletFactory* bFactory;
};