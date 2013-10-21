#pragma once
#include "Object.h"
#include "Log.h"

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
	Scene(sf::RenderWindow& window):window(window){}

	virtual void draw();
	virtual void process();
	void addObject(Object* object);

protected:
	sf::RenderWindow & window;
};