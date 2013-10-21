#pragma once
#include "Object.h"
#include <SFML\Window\Event.hpp>
#include "BulletFactory.h"
#include <functional>


/// <summary>
/// The player class represents the enemy
/// </summary>
class Enemy : public Object{
	std::function<void(Object*)> sceneObjCallBack;
	BulletFactory* bFactory;
	sf::Vector2f startPos;
	sf::Vector2f endPos;

public:
	Enemy(){}
	Enemy::Enemy(sf::RenderWindow& window, 
		sf::Vector2f startPos,
		sf::Vector2f endPos,
		float shootFrequency,
		int radius, 
		BulletFactory* bFactory, 
		std::function<void(Object*)> sceneObjCallBack);
	bool process();
	void test();

protected:
	void circularShoot();
	
};