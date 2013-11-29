#pragma once
#include "Object.h"
#include "ResourceHandler.h"
#include <memory>

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

	int getHealth(); // Todo should be protected
	Shooter::ShooterType getType();
	float playerScore;

	void setValue(float value);
	float getValue();
protected:

	Shooter(sf::RenderWindow& window, 
		BulletFactory&, 
		std::list<std::unique_ptr<Bullet>>&,
		std::unique_ptr<ResourceHandler>& resourceHandler,
		const sf::Time& timeStep);	
	int health;

	std::unique_ptr<ResourceHandler>& resourceHandler;

	void decrementHealth();
	void incrementHealth();


	/* Processing */
	virtual void shooterProcess();

	void setType(Shooter::ShooterType);


	// Bullets getter/setter
	std::list<std::unique_ptr<Bullet>>& getBullets();
	void setBullets(std::list<std::unique_ptr<Bullet>>& bullets);

	// BulletFactory Getter/Setter
	BulletFactory& getBulletFactory();
	const sf::Time& timeStep;

	/* Health Related */
	void setHealth(int value);

private:
	virtual void hitDetection() ;
	bool circleTest(GameShape& bullet);
	bool sat(std::shared_ptr<GameShape> c1, std::shared_ptr<GameShape> c2);
	float scoreValue;
	ShooterType shooterType;

	std::list<std::unique_ptr<Bullet>>& bullets;
	BulletFactory& bFactory;


};