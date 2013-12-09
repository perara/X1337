#pragma once
#include "Shooter.h"
#include "Powerup.h"

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
		std::shared_ptr<ResourceHandler>& resourceHandler,
		const sf::Time& timeStep,
		const bool hardmode
		);


	virtual void input(sf::Event&); // Input handler for player
	virtual void process();

	void addScore(float score); // Adds a X sum to player's score
	void drawStats(std::list<std::shared_ptr<HighScoreItem>>&);

	int getPlayerScore();

	void powerUp(Powerup::PowerUpType);
private:

	int playerScore;
	void detectEdge();
	void processPowerUps();

	// Timers and such for power ups
	sf::Clock pwrUpClock;

	// Pulsate
	bool pulsateGun;
	sf::Clock pulseClock;
};