#pragma once
#include "Scene.h";
#include <vector>

class Player;
class Background;
class Shooter;
class Bullet;
class Script;
class BulletFactory;

/// <summary>
/// World is the place where all encounters in game happen. It keeps record of the player, enemies and bullets. This is rendered and processed via the Scene inheritance.
/// </summary>
class World: public Scene{
public:
	std::vector<Shooter*> objects; 
	std::vector<Bullet*> bullets;
	BulletFactory* bFactory;

	void addObject(Shooter* object);
	void addBullet(Bullet* bullet);

	World();
	World(sf::RenderWindow& window);
	void drawStats();

	virtual void draw();
	virtual void process();
	virtual void init();
	virtual void input(sf::Event&);

private:
	Player* player;
	Background* bg;
	Script* getScript();
	void setScript(Script*);
	Script * script;
};