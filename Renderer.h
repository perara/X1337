#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "World.h"
#include "Player.h"
#pragma once
class Renderer
{
public:
	bool isWindowOpen();
	bool pollEvent(sf::Event& e);
	void render();
	sf::RenderWindow window;
	World& w;
	Renderer(World& w);
	~Renderer(void);
};

