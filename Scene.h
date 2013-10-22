#pragma once
#include "Object.h"
#include "Log.h"
#include "Bullet.h"

#include <deque>
#include <list>
#include <iostream>
#include <vector>
#include <SFML\Graphics\CircleShape.hpp>
#include <SFML\Graphics\RenderWindow.hpp>


/// <summary>
/// The scene class is an abstract class which is base class for all States. All state is a Scene.
/// </summary>
class Scene{
public:
	std::list<Object*> objects; // Reason for using List here would be to not invalidate iterator on insertion. Need input
	std::list<Bullet*> bullets;
	Scene(sf::RenderWindow& window):window(window){

		//TODO?
		Bullet* initB = new Bullet(window,1);
		initB->setPosition(-10,-10);
		bullets.push_back(initB); // This have to be here in order to keep the std::list at a size of 1. 
	}

	virtual void draw();
	virtual void process();
	void addObject(Object* object);
	void addBullet(Bullet* bullet);

protected:
	sf::RenderWindow & window;
};