#pragma once
#include <SFML\Graphics\Image.hpp>
#include <SFML\Graphics\Texture.hpp>
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\CircleShape.hpp>
#include <queue>
#include <list>

class Background
{
	std::queue<sf::Sprite> images;
	std::list<sf::Texture*> textures;
	sf::Sprite currentImage;
	int scrollOffset;
	sf::RenderWindow& window;

public:
	Background(sf::RenderWindow& window);
	void addBackground(sf::Texture*);
	void process();
	void draw();

};