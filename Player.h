#pragma once
#include "Object.h"
#include <SFML\Window\Event.hpp>
#include "BulletFactory.h"
#include <functional>


/// <summary>
/// The player class represents the player
/// </summary>
class Player : public Object{
	std::function<void(Object*)> sceneObjCallBack;
	BulletFactory* bFactory;

public:
	Player(){}
	Player(sf::RenderWindow& window, 
		sf::Vector2f pos, 
		int radius,
		BulletFactory* bFactory,
		std::function<void(Object*)> sceneObjCallBack);
	void inputHandler();
	bool process();


};