#include "Background.h"
#include "Globals.h"

Background::Background(sf::RenderWindow& window): window(window)
{
}


void Background::addBackground(sf::Texture* texture)
{
	// Create the sprite
	sf::Sprite sprite;
	sprite.setColor(sf::Color(255,0,255)); // TEMP
	sprite.scale(((float)Globals::getInstance().getGameView().getSize().x / texture->getSize().x) , ((float)Globals::getInstance().getGameView().getSize().y / texture->getSize().y));
	textures.push_back(texture);

	if(currentImage.getLocalBounds().height == 0)
	{
		currentImage = sprite;
	}
	else
	{
		images.push(sprite);
	}
}

void Background::process()
{
	this->draw();
}

void Background::draw()
{
	if(!textures.empty()){
		currentImage.setTexture(*textures.front());
		currentImage.setPosition(0,0);
		//std::cout << "Height: " << currentImage.getLocalBounds().height << "| Width: " << currentImage.getLocalBounds().width << std::endl;
		this->window.draw(currentImage);
	}

}