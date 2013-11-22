#include "ResourceHandler.h"
#include "Log.h"
#include "Enemy.h"

ResourceHandler::ResourceHandler(sf::RenderWindow& window):
	window(window)
{
	this->setInit(false);
}


void ResourceHandler::init()
{
	// Define Resources
	// Textures
	{
		textureList[Texture::BACKGROUND1]  = "assets/images/background1.jpg";
	}

	// Sounds
	{
		soundList[Sound::SONG1]  = "assets/images/background1.jpg";
	}

	// Scripts
	{
		scriptList[Scripts::ENCOUNTER1] = "assets/scripts/test.xml";
	}


	this->loadFont();
	this->draw();
	this->loadTextures();
	this->loadSound();
	this->loadScripts();



	this->setInit(true);
}


void ResourceHandler::loadFont()
{
	// Load fonts
	font.loadFromFile("assets/fonts/COMICATE.ttf");
}


void ResourceHandler::loadTextures()
{
	// Load resources
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
	for(auto& i: soundList)
	{
		if (sounds[i.first].loadFromFile(i.second)){
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

	for(auto& i : scriptList)
	{
		Script * script = new Script();
		std::queue<sf::Vector3f> pathQueue;


		std::ifstream theFile (i.second);
		std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
		buffer.push_back('\0');

		rapidxml::xml_document<> doc;

		// Parse XML file
		doc.parse<0>(&buffer[0]);

		// Root Node
		rapidxml::xml_node<> *node = doc.first_node("Enemies");

		int enemyCounter = 0; // Counter
		for (rapidxml::xml_node<> *enemy = node->first_node(); enemy; enemy = enemy->next_sibling())
		{
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

			/*Enemy* e1 = new Enemy(
				window, 
				pathQueue,
				10, 
				bFactory,
				bullets
				);
				*/
			enemyCounter++;
		}
	}

}




sf::Texture* ResourceHandler::getTexture(ResourceHandler::Texture res)
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


