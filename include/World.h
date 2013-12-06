#pragma once
#include "Scene.h"
#include "Background.h"
#include "BulletFactory.h"
#include "Player.h"
#include "Script.h"
#include "ResourceHandler.h"
#include <memory>


/// <summary>
/// World is the place where all encounters in game happen. It keeps record of the player, enemies and bullets. This is rendered and processed via the Scene inheritance.
/// </summary>
class World : public Scene{
public:

	World();
	~World();
	World(sf::RenderWindow& window,
		std::unique_ptr<ResourceHandler>& resourceHandler,
		const sf::Time& timeStep,
		const bool demo,
		const int scriptNum,
		const bool hardMode,
		sf::Sound& ingameSong);

	void drawStats();
	bool isGameOver();

	virtual void draw();
	virtual void process();
	virtual void input(sf::Event&);

	void startSound();
	void stopSound();

	BulletFactory& getBulletFactory()
	{
		return bFactory;
	};

	sf::Sound& ingameSong; // Game song which loops.
private:
	// Objects
	Script script;
	Background bg;
	BulletFactory bFactory;

	sf::Sound& countdownSong; // Countdown for game start
	std::shared_ptr<Player> player;

	const sf::Time& timeStep;
	bool demo;

	const bool getHardMode();
	const bool hardMode;

	bool gameOver;

	int currentScript;

	std::list<std::shared_ptr<Shooter>> objects;
	std::list<std::unique_ptr<Bullet>> bullets;

	void addObject(std::shared_ptr<Shooter> object);
	void addBullet(std::unique_ptr<Bullet> bullet);
};