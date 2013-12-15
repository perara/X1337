#pragma once
#include "Shooter.h"
#include "Powerup.h"

class BulletFactory;
class Bullet;


/// <summary>
/// The Player class represents the player. The Player derived from Shooter which is common for both Enemy and Player
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

	// Adds a X sum to player's score
	void addScore(float score);

	// Draw the stats view
	void drawStats(std::list<std::shared_ptr<HighScoreItem>>&);

	// Retrieves the player score
	int getPlayerScore();

	// Retrieves the player kills (number of enemy kills)
	int getPlayerKills();

	// Sets a powerup for the player (Func)
	void powerUp(Powerup::PowerUpType);
private:
	/// The player score
	int playerScore;

	// Enemy Kills
	int playerKills;

	// Detects window edges (basicly window hit detection)
	void detectEdge();

	// Process powerup loop
	void processPowerUps();

	/// Timers and such for power ups
	sf::Clock pwrUpClock;

	/// Pulsate
	bool pulsateGun;
	/// Pulsate clock
	sf::Clock pulseClock;
};