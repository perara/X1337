#pragma once
#include "Scene.h"
#include "Background.h"
#include "BulletFactory.h"
#include "Player.h"
#include "ScriptTemplate.h"
#include "ResourceManager.h"
#include "Powerup.h"
#include "Script.h"
#include "Renderer.h"

#include <memory>

/// <summary>
/// World class is a construction which runs Script and Player. It has a Input, Process, and Draw Function for this purpose.
/// </summary>
class World : public Scene{
public:

	// Constructor and Deconstructor
	World(Renderer& window,
          std::shared_ptr<ResourceManager>& resourceHandler,
          const sf::Time& timeStep,
          const Constants::ResourceC::Scripts scriptNum,
          const bool hardMode,
          sf::Sound& ingameSong);
	~World();

	// Virtuals (Derived from Scene)
	virtual void draw();
	virtual void process();
	virtual void input(sf::Event&);

	void drawStats(); // Draw Player stats
	int isGameOver() const;
	void startSound();
	void stopSound();

	BulletFactory& getBulletFactory();
	const sf::Time& getTimeStep();
    std::list<std::unique_ptr<Bullet>>& getBullets();
    std::shared_ptr<ResourceManager>& getResourceHandler();
    sf::RenderWindow& getWindow();


    void addEnemyObject(std::shared_ptr<Enemy> &enemy);

    std::list<std::shared_ptr<Shooter>> & getShooterObjects();

    Renderer &getRenderer();

    void addPowerupObject(std::shared_ptr<Powerup> &powerup);

private:
	std::unique_ptr<Script> script;
	Background bg;
	BulletFactory bFactory;
	std::shared_ptr<Player> player;

	sf::Sound& ingameSong; // In-game sound
	sf::Sound& countdownSong; // Countdown for game start


	const sf::Time timeStep;

	const bool hardMode;
	int gameOver;

	float stageProgress; // Displays current stage progress as a percentage

	std::list<std::shared_ptr<Shooter>> objects;
	std::list<std::unique_ptr<Bullet>> bullets;
	std::list<std::shared_ptr<Powerup>> powerups;

	void drawGameProgress();
	void evaluateGameOver();

    bool isGameMenu();

    bool hasGameScript();

    std::list<std::shared_ptr<Powerup>> &getPowerupObjects();
};