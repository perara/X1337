#include "ResourceHandler.h"

ResourceHandler::ResourceHandler(sf::RenderWindow& window):
	window(window)
{
	this->setInit(false);
}


void ResourceHandler::init()
{
	// Load fonts
	font.loadFromFile("assets/fonts/COMICATE.ttf");

	this->draw();
	// Init resources
	resources[Resource::BACKGROUND1]  = "assets/images/background1.jpg";

	// Load resources
	for(auto& i : resources)
	{
		if (textures[i.first].loadFromFile(i.second)){
			LOGD("Texture loaded: " << i.second);
		}
		else
		{
			LOGD("Failed to load texture: " << i.second);
			
		}
	}

	this->setInit(true);
}

sf::Texture* ResourceHandler::getTexture(ResourceHandler::Resource res)
{
	return &this->textures[res];
}



bool ResourceHandler::getInit()
{
	return this->inited;
}

void ResourceHandler::setInit(bool init)
{
	this->inited = init;
}


void ResourceHandler::draw()
{
	sf::Text label;
	label.setFont(this->font);
	label.setString(sf::String("Loading... Please Wait!"));
	label.setPosition((window.getPosition().x / 2) -  label.getGlobalBounds().width , (window.getPosition().y / 2) - label.getGlobalBounds().height);
	label.setColor(sf::Color::White);

	window.clear(sf::Color::Black);
	window.draw(label);
	window.display();



}


