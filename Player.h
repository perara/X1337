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
		int radius
		);

	void input(sf::Event&);
	void process();
	void detectEdge();
	void drawStats();
};