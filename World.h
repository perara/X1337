#pragma once
#include "Scene.h";

class BulletFactory;

/// <summary>
/// World is the place where all encounters in game happen. It keeps record of the player, enemies and bullets. This is rendered and processed via the Scene inheritance.
/// </summary>
class World: public Scene{
	BulletFactory* bFactory;

public:
	World();
	World(sf::RenderWindow& window);
	void process();
};