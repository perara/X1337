#pragma once
#include "Shooter.h"
#include "Bullet.h"
#include "GameGlobals.h"

/// <summary>
/// The player class represents the player
/// </summary>
class Player : public Shooter{
public:
	Player(sf::RenderWindow& window, 
		sf::Vector2f pos, 
		int radius,
		BulletFactory* bFactory,
		std::list<Bullet*>&bullets                                      //std::function<void(Bullet*)> sceneObjCallBack Deprecating this. need full access to bullets in players and enemies
		);
	void inputHandler();
	bool process();
};