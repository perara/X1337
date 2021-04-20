#pragma once
#include "Shooter.h"
#include "Powerup.h"
#include "Constants.h"
#include <memory>

class BulletFactory;
class Bullet;


/// <summary>
/// The Player class represents the player. The Player derived from Shooter which is common for both Enemy and Player
/// </summary>
class Player : public Shooter
{
public:
	Player(Renderer& window,
		sf::Vector2f pos,
		int radius,
		BulletFactory&,
		std::list<std::unique_ptr<Bullet>>&,
		std::shared_ptr<ResourceManager>& resourceHandler,
		const sf::Time& timeStep,
		const bool hardmode,
		std::list<std::shared_ptr<Shooter>>&
		);


	virtual void input(sf::Event&); // Input handler for player
	virtual void process();

	// Adds a X sum to player's score
	void addScore(float score);

	// Draw the stats view
	void drawStats(std::list<HighScoreItem>&);

	// Retrieves the player score
    float getPlayerScore() const;


	// Retrieves the player kills (number of enemy kills)
	int getPlayerKills() const;

	// Sets a powerup for the player (Func)
	void powerUp(Constants::PowerUpType);
private:

	/// The player score
	float playerScore;

	// Enemy Kills
	int playerKills;

	int radius;

	// Detects window edges (basicly window hit detection)
	void detectEdge();

	// Process powerup loop
	void processPowerUps();

	/// Timers and such for power ups
	sf::Time pwrUpTime;

	/// Pulsate
	bool pulsateGun;

	/// Pulsate clock
	sf::Time pulseTime;

	/// <summary>
	/// The normal shot clock
	/// </summary>
	sf::Time normalShotTime;

	/// <summary>
	/// The special shot clock
	/// </summary>
	sf::Time specialShotTime;

};