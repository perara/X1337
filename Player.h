#pragma once
#include "Shooter.h"


class BulletFactory;
class Bullet;


/// <summary>
/// The player class represents the player
/// </summary>
class Player : public Shooter
{
public:
	Player(sf::RenderWindow& window, 
		sf::Vector2f pos, 
		int radius,
		BulletFactory* bFactory,
		std::list<Bullet*>&bullets                                      //std::function<void(Bullet*)> sceneObjCallBack Deprecating this. need full access to bullets in players and enemies
		);

	void inputHandler();
	void process();
protected:
		virtual int hitDetection();
};