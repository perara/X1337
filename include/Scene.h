#pragma once
#include "ResourceHandler.h"
#include <SFML\Graphics.hpp>

/// <summary>
/// The scene class is an abstract class which is base class for all States. All state is a Scene.
/// </summary>
class Scene
{
public:
	Scene(sf::RenderWindow& window, std::shared_ptr<ResourceHandler>& resourceHandler) :
		window(window),
		resourceHandler(resourceHandler){};

	virtual void draw() = 0;
	virtual void process() = 0;
	virtual void input(sf::Event&) = 0;

protected:
	sf::RenderWindow& window;
	std::shared_ptr<ResourceHandler>& resourceHandler;
	bool inited;
};