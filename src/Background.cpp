#include "Background.h"
#include "Log.h"

Background::Background(sf::RenderWindow& window) : window(window)
{
	slide = false;
}


void Background::addBackground(sf::Texture& texture, bool slide)
{
	LOGD("Loading background...");
	this->slide = slide;
	// Create the sprite
	sf::Sprite sprite;
	//sprite.setColor(sf::Color(255, 0, 255)); // TEMP
	//sprite.scale(((float)window.getView().getSize().x / texture.getSize().x), /*((float)window.getView().getSize().y / texture.getSize().y)*/ 1);

	textures.push_back(std::make_shared<sf::Texture>(texture));

	// Check weither the texture is set or not already, if it is we just push the sprite the vector.
	if (
		primary.getGlobalBounds().height == 0 &&
		secondary.getGlobalBounds().height == 0)
	{
		primary = sprite;
		secondary = sprite;
	}
	else
	{
		images.push(sprite);
	}

	// Set the textures.
	primary.setTexture(*textures.front());
	secondary.setTexture(*textures.front());

	// Move secondary textureSlide above primary.
	secondary.setPosition(0, -secondary.getGlobalBounds().height);
	LOGD("Background loaded successfully.");
}

void Background::process()
{

	if (slide)
	{
		primary.move(0, 1);
		secondary.move(0, 1);
		if (secondary.getGlobalBounds().top >= secondary.getGlobalBounds().height)
		{
			secondary.setPosition(0, -secondary.getGlobalBounds().height);
		}


		if (primary.getGlobalBounds().top >= primary.getGlobalBounds().height)
		{
			primary.setPosition(0, -primary.getGlobalBounds().height);
		}
	}


}

void Background::draw()
{
	window.draw(primary);
	if (slide) window.draw(secondary);
}

