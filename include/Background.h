#pragma once
#include <SFML\Graphics\Image.hpp>
#include <SFML\Graphics\Texture.hpp>
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\CircleShape.hpp>
#include <queue>
#include <list>
#include <memory>

class Background
{
	std::queue<sf::Sprite> images;
	std::list<std::shared_ptr<sf::Texture>> textures;

	sf::Sprite primary;
	sf::Sprite secondary;
	bool slide;


	int scrollOffset;
	sf::RenderWindow& window;

public:
	Background(sf::RenderWindow& window);
	void addBackground(sf::Texture&, bool);
	void process();
	void draw();

};