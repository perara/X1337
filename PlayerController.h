#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Window.h"
#include "Player.h"
#pragma once
class PlayerController
{
public:
	void updateWorld();
	Window& w;
	Player& player;
	sf::Window& window;
	PlayerController(Window& w,Player& player, sf::Window& window);
	~PlayerController(void);
};