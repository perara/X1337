#pragma once
#include <vector>
#include <SFML\Graphics\CircleShape.hpp>
#include <SFML\Graphics\RenderWindow.hpp>

#include "Object.h"

class Scene{
public:
	std::vector<Object*> objects;
	Scene(sf::RenderWindow& window):window(window){}

	virtual void draw();
	virtual void process();
	void addObject(Object* object);

protected:
	sf::RenderWindow & window;
};