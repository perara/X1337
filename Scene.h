#pragma once
#include <vector>
#include <SFML\Graphics\CircleShape.hpp>
#include <SFML\Graphics\RenderWindow.hpp>

#include "Object.h"

class Scene{
public:
	std::vector<Object*> objects;

	virtual void draw(sf::RenderWindow &window);
	virtual void process();

	void addObject(Object* object);


};