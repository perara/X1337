#include "ResourceHandler.h"
#include "Log.h"
#include "Enemy.h"
#include <memory>
#include <sstream>

ResourceHandler::ResourceHandler(sf::RenderWindow& window):
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
		textureList[Texture::BACKGROUND1]  = "assets/sprites/bg1.jpg";
		textureList[Texture::HEART]  = "assets/sprites/heart.png";
		textureList[Texture::MITT]  = "assets/sprites/mitt-romney.png";
	}

	// Sounds
	{
		soundList[Sound::SONG1]  = "assets/sound/a.ogg";
		soundList[Sound::MENU_SONG] = "assets/sound/game_menu.ogg";
		soundList[Sound::STANDARD_SHOT] = "assets/sound/FireOneSound.ogg";
		soundList[Sound::HEAVY_SHOT] = "assets/sound/FireOneSound.ogg";
		soundList[Sound::ENEMY_DEATH] = "assets/sound/ExplosionSound.ogg";
		soundList[Sound::INGAME] = "assets/sound/in-game.ogg";
	}

	// Scripts
	{
		scriptList[Scripts::STAGEONE] = "assets/scripts/stage1.xml";
		scriptList[Scripts::ENCOUNTER1] = "assets/scripts/encounterDemo.xml";
		scriptList[Scripts::ENCOUNTER2] = "assets/scripts/encounter.xml";
		scriptList[Scripts::ENCOUNTER3] = "assets/scripts/encounter.xml";
		scriptList[Scripts::GAME_MENU] = "assets/scripts/game_menu.xml";
		scriptList[Scripts::DRIT] = "assets/scripts/drit.xml";
		scriptList[Scripts::DRIT1] = "assets/scripts/drit1.xml";
		scriptList[Scripts::DRIT2] = "assets/scripts/drit2.xml";
		scriptList[Scripts::DRIT3] = "assets/scripts/drit3.xml";

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
	std::ifstream theFile (highScoreFile);
	std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
	buffer.push_back('\0');

	rapidxml::xml_document<> doc;

	// Parse XML file
	doc.parse<0>(&buffer[0]);

	// Root Node
	rapidxml::xml_node<> *root = doc.first_node("Savegame");

	// Parse Paths
	rapidxml::xml_node<> *stages = root->first_node("Stages");

	//Stages nodes
	for (rapidxml::xml_node<> *stage = stages->first_node(); stage; stage = stage->next_sibling())
	{
		int stageEnum = atoi(stage->first_attribute("enum")->value());

		std::list<std::shared_ptr<HighScoreItem>> highScoreStage; // Single Stage
		for(rapidxml::xml_node<> *player = stage->first_node("Player"); player; player = player->next_sibling())
		{
			//player = stage->first_node("Player");
			std::string pName = player->first_node("Name")->value();
			float pScore = atof(player->first_node("Score")->value());
			std::string date = player->first_node("Date")->value();
			highScoreStage.push_back(std::shared_ptr<HighScoreItem>(new HighScoreItem((ResourceHandler::Scripts)stageEnum, pName, pScore, date)));
		}

		// Sort by score
		highScoreStage.sort([]( std::shared_ptr<HighScoreItem> & a,  std::shared_ptr<HighScoreItem> & b) { return a->score > b->score; });

		highScoreStages[(ResourceHandler::Scripts)stageEnum] = (highScoreStage);
	}
}

void ResourceHandler::writeHighScoreScore(int score, int scriptEnum)
{
	// LOAD FILE
	rapidxml::xml_document<> doc;
	std::ifstream theFile (highScoreFile);
	std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>( ));
	buffer.push_back('\0');
	doc.parse<0>(&buffer[0]);

	// Parse new info
	std::stringstream scoreStr;//create a stringstream
	scoreStr << score;//add number to the stream
	std::string src2 = "<Player><Name>NO-NAME</Name><Score>" + scoreStr.str() + "</Score><Date>N/A</Date></Player>";
	std::vector<char> x(src2.begin(), src2.end());
	x.push_back( 0 ); // make it zero-terminated as per RapidXml's docs

	// Enum to string
	std::stringstream enumStr;//create a stringstream
	enumStr << scriptEnum;//add number to the stream

	std::string src3 = "<Stage enum=\""+ enumStr.str() +"\"><Player><Name>NO-NAME</Name><Score>" + scoreStr.str() + "</Score><Date>N/A</Date></Player></Stage>";
	std::vector<char> x2(src3.begin(), src3.end());
	x2.push_back( 0 ); // make it zero-terminated as per RapidXml's docs


	rapidxml::xml_document<> xmlseg; // New PlayerNode
	xmlseg.parse<0>( &x[0] );

	rapidxml::xml_document<> xmlseg2; // sTAGEnODE
	xmlseg2.parse<0>( &x2[0] );


	// get document's first node - 'SAVEGAME' node
	// get player's first child - 'STAGES' node
	// get frames' first child - first 'STAGE' node
	rapidxml::xml_node<>* nodeFrame = doc.first_node()->first_node()->first_node();
	rapidxml::xml_node<>* searchNode = nullptr;

	// Loop through all stage nodes.
	while(nodeFrame)
	{
		std::stringstream ss;
		ss << nodeFrame->first_attribute("enum")->value();
		int enumVal;
		ss >> enumVal;

		if(enumVal == scriptEnum)
		{
			searchNode = nodeFrame;
			break;

		}
		nodeFrame = nodeFrame->next_sibling();
	}

	// Check if the node with corresponding enum was found, if it was not we will create a new Stage Node
	if(searchNode != nullptr)
	{
		searchNode->append_node(xmlseg.first_node());
	}
	else
	{
		doc.first_node()->first_node()->append_node(xmlseg2.first_node());
	}

	// Parse data to a string 
	std::string data;
	rapidxml::print(std::back_inserter(data), doc);

	// Write to file (Overwrite)
	std::ofstream file;
	file.open(highScoreFile.c_str());
	file << data;
	file.close();


	loadHighScore(); // RELOAD  XML
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
		rapidxml::xml_node<> *root = doc.first_node("Script");

		// Root Node
		rapidxml::xml_node<> *node = root->first_node("Enemies");

		// Get repeat node
		rapidxml::xml_node<> *repeat = root->first_node("Repeat");

		// Get name node
		rapidxml::xml_node<> *name = root->first_node("Name");
		LOGD(name->value());

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

			//std::cout << e1 << std::endl;
			std::string nameCpp(name->value()); // Convert name to CPP11 format
			this->scripts[i.first].setScriptEnumVal(i.first);
			this->scripts[i.first].setScriptTitle(nameCpp);
			this->scripts[i.first].addEnemy(delay, pathQueue, type, atoi(repeat->value()));

			enemyCounter++;
		}

		this->scripts[i.first].setInit(true); // Set script init to true
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
	for(Script& i : getScripts())
	{
		if (cnt == iteNum) return i;
		cnt++;
	}
}

std::list<Script> ResourceHandler::getScripts()
{
	std::list<Script> ret;
	for(Script i : scripts)
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
		window.getView().getCenter().x  -  (label.getGlobalBounds().width / 2) , 
		window.getView().getCenter().y /2 - (label.getGlobalBounds().height / 2));
	label.setColor(sf::Color::White);


	window.clear(sf::Color::Black);
	window.draw(label);
	window.display();



}


