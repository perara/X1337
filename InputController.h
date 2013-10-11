#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "World.h"
#pragma once
class InputController
{
public:
	void updateWorld();
	World& w;
	sf::Window& window;
	InputController(World& w, sf::Window& window);
	~InputController(void);
};