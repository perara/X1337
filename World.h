#pragma once
#include "Scene.h"
#include "Background.h"
#include "BulletFactory.h"
#include "Player.h"
#include "Script.h"


class Shooter;
class Bullet;


/// <summary>
/// World is the place where all encounters in game happen. It keeps record of the player, enemies and bullets. This is rendered and processed via the Scene inheritance.
/// </summary>
class World: public Scene{
public:
	std::list<Shooter*> objects; 
	std::list<Bullet*> bullets;

	void addObject(Shooter* object);
	void addBullet(Bullet* bullet);

	World();
	World(sf::RenderWindow& window);
	void drawStats();

	virtual void draw();
	virtual void process();
	virtual void init();
	virtual void init(int scriptNum);
	virtual void input(sf::Event&);
	virtual void reset();

	bool isDemo();
	void setDemo(bool);
	BulletFactory& getBulletFactory()
	{
		return bFactory;
	};

private:
	// Objects
	Background bg;
	BulletFactory bFactory;
	Player player;

	// Script pointers
	Script script;

	bool demo;
};