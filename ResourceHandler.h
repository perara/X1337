#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include "log.h"


class ResourceHandler
{
public:
	const enum Resource{
		BACKGROUND1,
		COUNT
	};

	ResourceHandler(sf::RenderWindow& window);


	void init();
	void draw();
	void setInit(bool);
	bool getInit();

	sf::Texture* getTexture(ResourceHandler::Resource);


protected:
	sf::RenderWindow& window;
	bool inited;
	std::map<Resource, std::string> resources;
	sf::Texture textures[Resource::COUNT];
	sf::Font font;
	
};
