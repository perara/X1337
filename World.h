#pragma once
#include "Scene.h"
#include "Player.h"

#include <iostream>
#include <functional>


/// <summary>
/// World is the place where all encounters in game happen. It keeps record of the player, enemies and bullets. This is rendered and processed via the Scene inheritance.
/// </summary>
class World: public Scene{
	BulletFactory* bFactory;
public:
	World();
	World(sf::RenderWindow& window);

	void process();

	// TODO
	sf::Vector2f GetSpriteCenter (const sf::CircleShape& Object);
	sf::Vector2f GetSpriteSize (const sf::CircleShape& Object);
	bool CircleTest(const sf::CircleShape& Object1, const sf::CircleShape& Object2);

private:
	void deleteBullet(std::list<Bullet*>::iterator& i);
};