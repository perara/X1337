#include "ResourceHandler.h"


struct TextureMapping
{
	ResourceHandler::Texture texture;
	const char* const path;
};

ResourceHandler::ResourceHandler(sf::RenderWindow& window):
	window(window)
{ 
	inited = false; 

}

bool ResourceHandler::init()
{
	// Font loading 
	LOGI("Loading font...");
	font.loadFromFile("assets/fonts/COMICATE.TTF");
	this->draw();  // Draw Loading screen


	static struct TextureMapping textureList[] =
	{
		{ Texture::BACKGROUND1,	"assets/images/background1.jpg" },
	};

	if (inited)
	{
		LOGW("Texture manager has already been initialized");
		return true;
	}

	LOGI("Loading textures...");

	for (size_t i = 0; i < (int)Texture::COUNT; i++)
	{
		if (textures[(size_t)textureList[i].texture].loadFromFile(textureList[i].path))
			LOGD("Texture loaded: " << textureList[i].path);
		else
		{
			LOGE("Failed to load texture: " << textureList[i].path);
			return false;
		}
	}


	inited = true;

	return true;
}

const sf::Texture& ResourceHandler::GetTexture(ResourceHandler::Texture textureId) const
{
	LOGD("Getting texture: " << (size_t)textureId);

	if (textures[(size_t)textureId].getSize().x == 0)
		throw std::runtime_error("Attempting to fetch empty texture");

	return textures[(size_t)textureId];
}

const sf::Font& ResourceHandler::GetFont() const
{
	return font;
}

void ResourceHandler::draw()
{
	sf::Text label;
	label.setFont(this->GetFont());
	label.setString(sf::String("Loading... Please Wait!"));
	label.setPosition((window.getPosition().x / 2) -  label.getGlobalBounds().width , (window.getPosition().y / 2) - label.getGlobalBounds().height);
	label.setColor(sf::Color::White);

	window.clear(sf::Color::Black);
	window.draw(label);
	window.display();



}


