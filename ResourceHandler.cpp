#include "ResourceHandler.h"
#include "Log.h"

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
		//scriptList[Scripts::ENCOUNTER1] = "";
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

	std::ifstream theFile ("assets/scripts/test.xml");
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
		// Variables
		int x, y, type, delay;
		bool shoot;

		type = atoi(enemy->first_node("Type")->value());
		delay = atoi(enemy->first_node("Delay")->value());

		// Output data:
		std::cout << "Enemy #" << enemyCounter << ":\n\t" << "Type: " << type << "\n\tDelay: " << delay << 
			"\n\t Paths: " << std::endl;

		// Parse Paths
		rapidxml::xml_node<> *node = enemy->first_node("Path");
		for (rapidxml::xml_node<> *child = node->first_node(); child; child = child->next_sibling())
		{
			x = atoi(child->first_attribute("x")->value());
			y = atoi(child->first_attribute("y")->value());
			shoot = boost::lexical_cast<bool>(child->first_attribute("shoot")->value());
			std::cout << "\n\t\tX: " << x <<
				"\n\t\tY: " << y <<
				"\n\t\tShoot: " << shoot << std::endl;

		}

		enemyCounter++;
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


