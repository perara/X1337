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
class World: public Scene{
public:
	std::list<std::shared_ptr<Shooter>> objects; 
	std::list<std::unique_ptr<Bullet>> bullets;

	void addObject(std::shared_ptr<Shooter> object);
	void addBullet(std::unique_ptr<Bullet> bullet);

	World();
	~World();
	World(sf::RenderWindow& window, 
		std::unique_ptr<ResourceHandler>& resourceHandler,
		const sf::Time& timeStep);

	void drawStats();

	virtual void draw();
	virtual void process();
	virtual void init(bool demo, int  scriptNum = (int)ResourceHandler::Scripts::GAME_MENU);
	virtual void input(sf::Event&);

	BulletFactory& getBulletFactory()
	{
		return bFactory;
	};

private:
	// Objects
	Background bg;
	BulletFactory bFactory;
	std::shared_ptr<Player> player;
	Script script;
	const sf::Time& timeStep;
	bool demo;
	sf::Sound& ingameSong;
};