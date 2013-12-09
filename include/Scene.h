#pragma once
#include "ResourceHandler.h"
#include <SFML\Graphics.hpp>

/// <summary>
/// The scene class is an abstract class which is base class for all States. All state is a Scene.
/// </summary>
class Scene
{
public:
	/// Constructor for scene which take constructs the scene object. 
	Scene(sf::RenderWindow& window, std::shared_ptr<ResourceHandler>& resourceHandler) :
		window(window),
		resourceHandler(resourceHandler){};

	/// Pure virtual draw handler
	virtual void draw() = 0;

	/// Pure virtual process handler
	virtual void process() = 0;

	/// Pure virtual input handler
	virtual void input(sf::Event&) = 0;

protected:
	/// This is a reference to the render window made in GameEngine
	sf::RenderWindow& window;

	/// ResourceHandler reference as a smart pointer which was made in GameEngine
	std::shared_ptr<ResourceHandler>& resourceHandler;

	/// This bool defines current state of the scene, weither its initialized or not.
	bool inited;
};