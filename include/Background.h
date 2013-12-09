#pragma once
#include <SFML\Graphics\Image.hpp>
#include <SFML\Graphics\Texture.hpp>
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\CircleShape.hpp>
#include <queue>
#include <list>
#include <memory>

/// <summary>
/// Background is used in World, and has it purpose to display and/or animate the background
/// </summary>
class Background
{
public:
	Background(sf::RenderWindow& window);
	
	void addBackground(sf::Texture&, bool);
	void process();
	void draw();

private:	
	sf::RenderWindow& window;

	std::queue<sf::Sprite> images;
	std::list<std::shared_ptr<sf::Texture>> textures;

	sf::Sprite primary;
	sf::Sprite secondary;
	
	int scrollOffset;
	bool slide;
};