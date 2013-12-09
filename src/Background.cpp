#include "Background.h"
#include "Log.h"

/// <summary>
/// Initializes a new instance of the <see cref="Background"/> class.
/// </summary>
/// <param name="window">The window.</param>
Background::Background(sf::RenderWindow& window) : 
window(window),
slide(false){}


/// <summary>
/// Adds a new background
/// </summary>
/// <param name="texture">The desired texture.</param>
/// <param name="slide">Boolean detrmining slide effect</param>
void Background::addBackground(sf::Texture& texture, bool slide)
{
	LOGD("Loading background...");
	this->slide = slide;

	// Create the sprite
	sf::Sprite sprite;
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

/// <summary>
/// Processes the background animation
/// </summary>
void Background::process()
{
	// Checks if the background is supposed to slide.
	if (slide)
	{
		// Move both images down a paxel.
		primary.move(0, 1);
		secondary.move(0, 1);
		
		// Checks if the image has reached out of bounds position
		if (secondary.getGlobalBounds().top >= secondary.getGlobalBounds().height)
		{
			// Resets the position
			secondary.setPosition(0, -secondary.getGlobalBounds().height);
		}

		// Checks if the image has reached out of bounds position
		if (primary.getGlobalBounds().top >= primary.getGlobalBounds().height)
		{
			//Resets the position
			primary.setPosition(0, -primary.getGlobalBounds().height);
		}
	}


}

/// <summary>
/// Draws the background image
/// </summary>
void Background::draw()
{
	// Draw primary background
	window.draw(primary);

	// Check if secondary should be drawn, and draws if true.
	if (slide) window.draw(secondary);
}

