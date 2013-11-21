#pragma once
#include <stdexcept>
#include <SFML/Graphics.hpp>


#include "log.h"


class ResourceHandler
{
public:
	// Texture identifier
	enum class Texture
	{
		BACKGROUND1,
		COUNT
	};

	ResourceHandler(sf::RenderWindow& window);
	void draw();

	bool init();
	const sf::Texture& GetTexture(Texture textureId) const;

	// Get pointer to pre-loaded font
	const sf::Font& GetFont() const;

protected:
	sf::Texture textures[(size_t)Texture::COUNT];
	sf::Font font;
	bool inited;
	sf::RenderWindow& window;
};
