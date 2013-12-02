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
		const sf::Time& timeStep,
		const bool hardmode
		);

	void addScore(float score); // Adds a X sum to player's score
	void input(sf::Event&); // Input handler for player
	virtual void process();
	void drawStats(std::list<std::shared_ptr<HighScoreItem>>&);
	int getPlayerScore();
private:
	int playerScore;
	void detectEdge();
};