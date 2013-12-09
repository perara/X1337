#pragma once
#include "Scene.h"
#include "Background.h"
#include "BulletFactory.h"
#include "Player.h"
#include "Script.h"
#include "ResourceHandler.h"
#include <memory>
#include "Powerup.h"


class World : public Scene{
public:

	// Constructor and Deconstructor
	World();
	World(sf::RenderWindow& window,
		std::shared_ptr<ResourceHandler>& resourceHandler,
		const sf::Time& timeStep,
		const bool demo,
		const int scriptNum,
		const bool hardMode,
		sf::Sound& ingameSong);
	~World();

	// Virtuals (Derived from Scene)
	virtual void draw();
	virtual void process();
	virtual void input(sf::Event&);

	void drawStats(); // Draw Player stats
	bool isGameOver();
	void startSound();
	void stopSound();

private:
	Script script;
	Background bg;
	BulletFactory bFactory;
	std::shared_ptr<Player> player;

	sf::Sound& ingameSong; // In-game sound
	sf::Sound& countdownSong; // Countdown for game start


	const sf::Time timeStep;

	const bool hardMode;
	const bool demo;
	bool gameOver;

	int currentScript;

	std::list<std::shared_ptr<Shooter>> objects;
	std::list<std::unique_ptr<Bullet>> bullets;
	std::list<std::shared_ptr<Powerup>> powerups;
};