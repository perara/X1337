#pragma once
#include "Scene.h"
#include "Player.h"
#include "Enemy.h"

/// <summary>
/// World is the place where all encounters in game happen. It keeps record of the player, enemies and bullets. This is rendered and processed via the Scene inheritance.
/// </summary>
class World: public Scene{
	BulletFactory* bFactory;
public:
	World();
	World(sf::RenderWindow& window);

	void process();
	void deleteBullet(std::list<Bullet*>::iterator& i);
};