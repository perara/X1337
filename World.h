#pragma once
#include "Scene.h"
#include "Player.h"

#include <iostream>
#include <functional>


class World: public Scene{
	BulletFactory* bFactory;
public:
	World();
	World(sf::RenderWindow& window);

	void process();

};