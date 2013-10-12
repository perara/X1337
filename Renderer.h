#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Window.h"
#include "Player.h"

#pragma once
class Renderer
{
public:
	bool isWindowOpen();
	bool pollEvent(sf::Event& e);
	void render();
	sf::RenderWindow window;
	Window& w;
	Player& player;
	Renderer(Window& w, Player& player);
	~Renderer(void);
};

