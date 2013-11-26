#include "ResourceHandler.h"
#include "Log.h"
#include "Enemy.h"
#include <memory>
#include "Globals.h"

ResourceHandler::ResourceHandler(sf::RenderWindow& window):
	window(window)
{
	this->setInit(false);
}

ResourceHandler::~ResourceHandler()
{
	// Delete sound buffer
	for(auto&i : sBufferList)
	{
		delete i;
	}

}

void ResourceHandler::init()
{
	// Define Resources
	// Textures
	{
		textureList[Texture::BACKGROUND1]  = "assets/sprites/bg1.jpg";
		textureList[Texture::HEART]  = "assets/sprites/heart.png";
	}

	// Sounds
	{
		soundList[Sound::SONG1]  = "assets/sound/a.ogg";
		soundList[Sound::MENU_SONG] = "assets/sound/game_menu.ogg";
	}

	// Scripts
	{
		scriptList[Scripts::ENCOUNTER1] = "assets/scripts/encounterDemo.xml";
		scriptList[Scripts::ENCOUNTER2] = "assets/scripts/encounter.xml";
		scriptList[Scripts::ENCOUNTER3] = "assets/scripts/encounter.xml";
		scriptList[Scripts::GAME_MENU] = "assets/scripts/game_menu.xml";
	}

	// Fonts
	{
		fontList[Fonts::COMICATE] = "assets/fonts/COMICATE.ttf";
		fontList[Fonts::SANSATION] = "assets/fonts/sansation.ttf";
	}


	this->loadFonts();
	this->draw();
	this->loadTextures();
	this->loadSound();
	this->loadScripts();



	this->setInit(true);
}

void ResourceHandler::loadFonts()
{
	// Load Fonts
	for(auto& i : fontList)
	{
		if (fonts[i.first].loadFromFile(i.second)){
			LOGD("Font loaded: " << i.second);
		}
		else
		{
			LOGD("Failed to load font: " << i.second);

		}
	}
}



void ResourceHandler::loadTextures()
{
	// Load Textures
	for(auto& i : textureList)
	{

		if (textures[i.first].loadFromFile(i.second)){
			LOGD("Texture loaded: " << i.second);
		}
		else
		{
			LOGD("Failed to load texture: " << i.second);

		}
	}
}

void ResourceHandler::loadSound()
{
	// Load sounds
	for(auto& i: soundList)
	{
		sf::SoundBuffer* buf = new sf::SoundBuffer;
		sBufferList.push_back(buf);
		if (buf->loadFromFile(i.second)){
			sounds[i.first].setBuffer(*buf);
			LOGD("Sound loaded: " << i.second);
		}
		else
		{
			LOGD("Failed to load sound: " << i.second);

		}

	}
}

void ResourceHandler::loadScripts()
{
	// Load Scripts
	for(auto& i : scriptList)
	{
		std::ifstream theFile (i.second);
		std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
		buffer.push_back('\0');

		rapidxml::xml_document<> doc;

		// Parse XML file
		doc.parse<0>(&buffer[0]);

		// Root Node
		rapidxml::xml_node<> *node = doc.first_node("Enemies");

		// Get repeat node
		rapidxml::xml_node<> *repeat = doc.first_node("Repeat");

		int enemyCounter = 0; // Counter

		// Enemy X
		for (rapidxml::xml_node<> *enemy = node->first_node(); enemy; enemy = enemy->next_sibling())
		{
			std::queue<sf::Vector3f> pathQueue = std::queue<sf::Vector3f>();


			int type = atoi(enemy->first_node("Type")->value());
			int delay = atoi(enemy->first_node("Delay")->value());

			// Parse Paths
			rapidxml::xml_node<> *node = enemy->first_node("Path");
			for (rapidxml::xml_node<> *child = node->first_node(); child; child = child->next_sibling())
			{
				int x = atoi(child->first_attribute("x")->value());
				int y = atoi(child->first_attribute("y")->value());
				int shoot = atoi(child->first_attribute("shoot")->value());

				// Push path into queue
				pathQueue.push(sf::Vector3f(x,y,shoot));

			}

			Enemy* e1 = new Enemy(
				window, 
				pathQueue,
				type,
				atoi(repeat->value()));

			this->scripts[i.first].addEnemy(e1, delay);

			enemyCounter++;
		}

	}

}



/*
GETTERS
*/
sf::Texture& ResourceHandler::getTexture(ResourceHandler::Texture res)
{
	return this->textures[res];
}

Script* ResourceHandler::getScript(ResourceHandler::Scripts query)
{
	return &this->scripts[query];
}

sf::Font& ResourceHandler::getFont(ResourceHandler::Fonts query)
{
	return this->fonts[query];
}

sf::Sound& ResourceHandler::getSound(ResourceHandler::Sound query)
{
	return this->sounds[query];
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
	label.setFont(this->getFont(ResourceHandler::COMICATE));
	label.setString(sf::String("Loading... Please Wait!"));
	label.setPosition(
		Globals::getInstance().getGameView().getCenter().x  -  (label.getGlobalBounds().width / 2) , 
		Globals::getInstance().getGameView().getCenter().y /2 - (label.getGlobalBounds().height / 2));
	label.setColor(sf::Color::White);


	window.clear(sf::Color::Black);
	window.draw(label);
	window.display();



}


