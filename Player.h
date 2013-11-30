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
		BulletFactory&,   
		std::list<std::unique_ptr<Bullet>>&,
		std::unique_ptr<ResourceHandler>& resourceHandler,
		const sf::Time& timeStep
		);

	void addScore(float score);

	void setHealth(int value);
	void input(sf::Event&);
	void process();
	void detectEdge();
	void drawStats();
private:
	float playerScore;
};