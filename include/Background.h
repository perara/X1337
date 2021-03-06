#pragma once
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <queue>
#include <list>
#include <memory>
#include "Renderer.h"

/// <summary>
/// Background is used in World, and has it purpose to display and/or animate the background
/// </summary>
class Background
{
public:
	Background(Renderer& window);

	void addBackground(sf::Texture&, bool);
	void process();
	void draw();

private:
	Renderer& window;

	std::queue<sf::Sprite> images;
	std::list<std::shared_ptr<sf::Texture>> textures;

	sf::Sprite primary;
	sf::Sprite secondary;

	int scrollOffset;
	bool slide;

	/// Variables which is used to create a rainbow color on the background.
	int r;
	int g;
	int b;
};