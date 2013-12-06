#include "ResourceHandler.h"
#include "Log.h"
#include "Enemy.h"
#include <memory>
#include <sstream>
#include <Windows.h>
#include <chrono>
#include <ctime>



ResourceHandler::ResourceHandler(sf::RenderWindow& window) :
window(window)
{
	this->setInit(false);
}

ResourceHandler::~ResourceHandler()
{
}

void ResourceHandler::init()
{
	// Define Resources
	// HIGHSCORE
	highScoreFile = "assets/config.xml";

	// Textures
	{
		textureList[Texture::BACKGROUND1] = "assets/sprites/bg1.jpg";
		textureList[Texture::BACKGROUND2] = "assets/sprites/bg2.jpg";
		textureList[Texture::BACKGROUND3] = "assets/sprites/bg3.jpg";
		textureList[Texture::HEART] = "assets/sprites/heart.png";
		textureList[Texture::MITT] = "assets/sprites/mitt-romney.png";
		textureList[Texture::PLAYER_SHIP] = "assets/sprites/player_ship.png";
		textureList[Texture::ENEMY_SHIP] = "assets/sprites/enemy_ship.png";
		textureList[Texture::BOSS] = "assets/sprites/boss.png";
		textureList[Texture::HEAVY_SHOT_TEXTURE] = "assets/sprites/heavy_shot.jpg";
		textureList[Texture::BOSS_DEATHSTAR_TEXTURE] = "assets/sprites/deathStar.png";
		textureList[Texture::CHUBBY_SHIP_TEXTURE] = "assets/sprites/chubby.png";
	}

	// Sounds
	{
		soundList[Sound::SONG1]  = "assets/sound/a.ogg";
		soundList[Sound::MENU_SONG] = "assets/sound/game_menu.ogg";
		soundList[Sound::STANDARD_SHOT] = "assets/sound/FireOneSound.ogg";
		soundList[Sound::HEAVY_SHOT] = "assets/sound/FireOneSound.ogg";
		soundList[Sound::ENEMY_DEATH] = "assets/sound/ExplosionSound.ogg";
		soundList[Sound::INGAME] = "assets/sound/in-game.ogg";
		soundList[Sound::DEATH_STAR_THEME] = "assets/sound/death_star_theme.ogg";
	}

	// Scripts
	{
		scriptList[Scripts::STAGE_ONE] = "assets/scripts/stage1.xml";
		scriptList[Scripts::STAGE_TWO] = "assets/scripts/stage2.xml";
		scriptList[Scripts::GAME_MENU] = "assets/scripts/game_menu.xml";

		scriptList[Scripts::DEATH_STAR] = "assets/scripts/test1.xml";
		scriptList[Scripts::TEST2] = "assets/scripts/test2.xml";
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
	this->loadHighScore();
	this->loadUserName();
	this->loadCredits();



	this->setInit(true);
}

void ResourceHandler::loadFonts()
{
	// Load Fonts
	for (auto& i : fontList)
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
	for (auto& i : textureList)
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
	for (auto& i : soundList)
	{

		std::shared_ptr<sf::SoundBuffer> buf = std::shared_ptr<sf::SoundBuffer>(new sf::SoundBuffer());
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

void ResourceHandler::loadHighScore()
{
	// Read XML file raw (Create file Stream)
	std::ifstream fileStream(highScoreFile);

	if (fileStream.good())
	{
		// Load XML and parse it
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load(fileStream);

		// If XML was parsed successfully without errors
		if (result)
		{
			// Fetch the root node
			pugi::xml_node root = doc.child("Savegame");

			// Iterate though all stages
			for (pugi::xml_node node : root.child("Stages").children())
			{
				// Highscore list for "this" (node) stage
				std::list<std::shared_ptr<HighScoreItem>> highScoreStage;

				// Get the stage enum from attribute
				int sEnum = atoi(node.attribute("enum").value());

				// Iterate through all players in the stage
				for (pugi::xml_node recNode : node.children())
				{
					// Fetch each of the player details for the highscore record
					std::string pName = recNode.child("Name").child_value();
					float pScore = atof(recNode.child("Score").child_value());
					std::string pDate = recNode.child("Date").child_value();
					highScoreStage.push_back(std::shared_ptr<HighScoreItem>(
						new HighScoreItem((ResourceHandler::Scripts)sEnum, pName, pScore, pDate)));
				}

				// Sort by score (ASC) then add to highScoreMap
				highScoreStage.sort([](std::shared_ptr<HighScoreItem> & a, std::shared_ptr<HighScoreItem> & b) { return a->score > b->score; });
				highScoreStages[(ResourceHandler::Scripts)sEnum] = (highScoreStage);
			}
		}
		else
		{
			LOGD("Error description: " << result.description() << "\n");
		}
	}
}

void ResourceHandler::writeHighScoreScore(int score, int scriptEnum)
{
	// Read XML file raw (Create file Stream)
	std::ifstream fileStream(highScoreFile);

	if (fileStream.good())
	{
		// Load XML and parse it
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load(fileStream);

		// If XML was parsed successfully without errors
		if (result)
		{
			// Fetch the root node
			pugi::xml_node root = doc.child("Savegame").child("Stages");

			// Find the correct Stage
			pugi::xml_node stage = root.find_child_by_attribute("Stage", "enum", std::to_string(scriptEnum).c_str());

			// Check if the node handle is null or not
			if (stage.empty())
			{
				// If it was not found we create a new node with the value we wanted,
				root.append_child("Stage").append_attribute("enum").set_value(std::to_string(scriptEnum).c_str());

				// Trying to set stage again...
				stage = root.find_child_by_attribute("Stage", "enum", std::to_string(scriptEnum).c_str());
			}


			// Create a new node for the highscore record
			pugi::xml_node highScoreNode = stage.append_child("Player");

			//#######################################//
			//#Creation flow is the following:		#//
			//#1. Create main child X				#//
			//#2. Create a text child				#//
			//#3. set the value of the text child	#//
			//#######################################//
			// Create Name Node
			highScoreNode
				.append_child("Name")
				.append_child(pugi::node_pcdata)
				.set_value(getUserName().c_str());

			// Create Score Node
			highScoreNode
				.append_child("Score")
				.append_child(pugi::node_pcdata)
				.set_value(std::to_string(score).c_str());

			// Create Date Node
			highScoreNode
				.append_child("Date")
				.append_child(pugi::node_pcdata)
				.set_value(getDateTime().c_str());

			//doc.print(std::cout, "", pugi::format_raw);
			//std::cout << std::endl;
			doc.save_file(highScoreFile.c_str());
		}
		else
		{
			LOGD("Error description: " << result.description() << "\n");
		}
	}
	loadHighScore(); // RELOAD  XML
}

void ResourceHandler::loadScripts()
{
	// Load Scripts
	for (auto& i : scriptList)
	{
		// Read XML file raw (Create file Stream)
		std::ifstream fileStream(i.second);

		if (fileStream.good())
		{
			// Load XML and parse it
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load(fileStream);

			// If XML was parsed successfully without errors
			if (result)
			{
				// Set the root node
				pugi::xml_node root = doc.child("Script");

				int scriptRepeat = atoi(root.child("Repeat").child_value());
				std::string scriptName = root.child("Name").child_value();


				// Create a log counter
				int counter = 0;

				// Loop through each of the enemies
				for (pugi::xml_node node : root.child("Enemies").children("Enemy"))
				{
					// Create a queue for the path
					std::queue<sf::Vector3f> pathQueue = std::queue<sf::Vector3f>();

					// Retrieve enemy type and delay data
					int eType = atoi(node.child("Type").child_value());
					int eDelay = atoi(node.child("Delay").child_value());

					// Retrieve each of the Paths
					for (pugi::xml_node path : node.child("Path").children())
					{
						// Convert string to integer
						int x = atoi(path.attribute("x").value());
						int y = atoi(path.attribute("y").value());
						int shoot = atoi(path.attribute("shoot").value());

						// Push path into the queue
						pathQueue.push(sf::Vector3f(x, y, shoot));
					}

					// Add enemy to script
					this->scripts[i.first].addEnemy(eDelay, pathQueue, eType, scriptRepeat);
					counter++;
				}

				// Set enum and Script title, then set status to initialized
				this->scripts[i.first].setScriptEnumVal(i.first);
				this->scripts[i.first].setScriptTitle(scriptName);
				this->scripts[i.first].setInit(true);

				// Consider script for loaded
				LOGD(i.first << " was successfully loaded. " << counter << "enemies was queued.");
			}
			else
				// Handle a error message on exception
			{
				LOGD("Error description: " << result.description() << "\n");
			}
		}
	}
}

void ResourceHandler::loadUserName()
{
#define INFO_BUFFER_SIZE 32767
	TCHAR  infoBuf[INFO_BUFFER_SIZE];
	DWORD  bufCharCount = INFO_BUFFER_SIZE;

	// Get and display the user name.
	GetUserName(infoBuf, &bufCharCount);

	char ch[260];
	char DefChar = ' ';
	WideCharToMultiByte(CP_ACP, 0, infoBuf, -1, ch, 260, &DefChar, NULL);

	//A std:string  using the char* constructor.
	std::string ss(ch);

	this->userName = ch;
}

void ResourceHandler::loadCredits()
{
	std::ifstream fileStream(creditsFilePath);

	if (fileStream.good())
	{
		// Load XML and parse it
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load(fileStream);

		// If XML was parsed successfully without errors
		if (result)
		{
			// Get the root node
			pugi::xml_node root = doc.child("Credits");

			// Loop through each of the credits
			for (pugi::xml_node node : root.children())
			{
				// Create a empty list for names to be credited.
				std::list<std::string> credNames;

				// Iterate through all of the names in a credit category
				for (pugi::xml_node dataNode : node.children())
				{
					// Push credited name into list
					credNames.push_back(dataNode.child_value());
				}

				// Add the list to map index.
				creditsMap[node.name()] = credNames;
			}

		}
		else
			// Handle a error message on exception
		{
			LOGD("Error description: " << result.description() << "\n");
		}

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

Script ResourceHandler::getScript(ResourceHandler::Scripts query)
{
	return this->scripts[query];
}

Script ResourceHandler::getScriptById(int iteNum)
{
	// Meh method
	int cnt = 1;
	for (Script& i : getScripts())
	{
		if (cnt == iteNum) return i;
		cnt++;
	}
}

std::list<Script> ResourceHandler::getScripts()
{
	std::list<Script> ret;
	for (Script i : scripts)
	{
		ret.push_back(i);
	}
	return ret;
}

sf::Font& ResourceHandler::getFont(ResourceHandler::Fonts query)
{
	return this->fonts[query];
}

sf::Sound& ResourceHandler::getSound(ResourceHandler::Sound query)
{
	return this->sounds[query];
}

std::map<ResourceHandler::Scripts, std::list<std::shared_ptr<HighScoreItem>>> ResourceHandler::getHighScores()
{
	return this->highScoreStages;
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
		window.getView().getCenter().x - (label.getGlobalBounds().width / 2),
		window.getView().getCenter().y / 2 - (label.getGlobalBounds().height / 2));
	label.setColor(sf::Color::White);


	window.clear(sf::Color::Black);
	window.draw(label);
	window.display();



}


