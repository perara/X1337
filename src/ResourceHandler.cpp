#include "ResourceHandler.h"
#include "Log.h"
#include "Enemy.h"
#include <memory>
#include <sstream>
#include <Windows.h>
#include <chrono>
#include <ctime>
#include <iomanip> // put_time
#include "VectorN.h"



/// <summary>
/// Initializes a new instance of the <see cref="ResourceHandler"/> class.
/// </summary>
/// <param name="window">The window.</param>
ResourceHandler::ResourceHandler(sf::RenderWindow& window) :
window(window)
{
	this->setInit(false);
}

/// <summary>
/// Finalizes an instance of the <see cref="ResourceHandler"/> class.
/// </summary>
ResourceHandler::~ResourceHandler()
{
}

/// <summary>
/// Initializes the resource handler.
/// </summary>
void ResourceHandler::init()
{
	// Define Resources
	// HIGHSCORE
	creditsFilePath = "assets/credits.xml";
	highScoreFile = "assets/config.xml";

	// Textures
	{
		textureList[Texture::BACKGROUND2] = "assets/sprites/bg2.jpg";
		textureList[Texture::BACKGROUND3] = "assets/sprites/bg3.jpg";
		textureList[Texture::HEART] = "assets/sprites/heart.png";
		textureList[Texture::PLAYER_SHIP] = "assets/sprites/player_ship.png";
		textureList[Texture::ENEMY_SHIP] = "assets/sprites/enemy_ship.png";
		textureList[Texture::BOSS] = "assets/sprites/boss.png";
		textureList[Texture::HEAVY_SHOT_TEXTURE] = "assets/sprites/heavy_shot.jpg";
		textureList[Texture::REGULAR_BULLET_1] = "assets/sprites/regular_bullet_1.png";
		textureList[Texture::BOSS_DEATHSTAR_TEXTURE] = "assets/sprites/deathStar.png";
		textureList[Texture::CHUBBY_SHIP_TEXTURE] = "assets/sprites/chubby.png";

		textureList[Texture::AUDIO_ON] = "assets/sprites/audio-on.png";
		textureList[Texture::AUDIO_OFF] = "assets/sprites/audio-off.png";
		textureList[Texture::HEALTH_KIT] = "assets/sprites/health-kit.jpg";
		textureList[Texture::PULSE_GUN] = "assets/sprites/pulse-gun.png";
		textureList[Texture::PLAYER_BAR] = "assets/sprites/player-bar.png";

	}

	// Sounds
	{
		soundList[Sound::MUSIC_MENU_SONG] = "assets/sound/game_menu.ogg";
		soundList[Sound::FX_STANDARD_SHOT] = "assets/sound/FireOneSound.ogg";
		soundList[Sound::FX_HEAVY_SHOT] = "assets/sound/FireOneSound.ogg";
		soundList[Sound::FX_ENEMY_DEATH] = "assets/sound/ExplosionSound.ogg";
		soundList[Sound::MUSIC_INGAME] = "assets/sound/in-game.ogg";
		soundList[Sound::MUSIC_COUNTDOWN] = "assets/sound/countdown.ogg";
		soundList[Sound::MUSIC_DEATH_STAR_THEME] = "assets/sound/death_star_theme.ogg";
		soundList[Sound::FX_PICKUP_HEALTH] = "assets/sound/health_pickup.wav";
		soundList[Sound::FX_MENU_CLICK] = "assets/sound/menu_click.ogg";
		soundList[Sound::FX_MENU_RETURN] = "assets/sound/menu_return.ogg";
		soundList[Sound::FX_ERROR] = "assets/sound/fx_error.wav";

		soundList[Sound::EMOTE_DEATHSTAR_GREET] = "assets/sound/emote_death_star_greet.ogg";
		soundList[Sound::EMOTE_DEATHSTAR_DEATH] = "assets/sound/emote_death_star_death.ogg";
		soundList[Sound::EMOTE_DEATHSTAR_PERIODIC_1] = "assets/sound/emote_death_star_periodic_1.ogg";
		soundList[Sound::EMOTE_DEATHSTAR_PERIODIC_2] = "assets/sound/emote_death_star_periodic_2.ogg";
		soundList[Sound::EMOTE_DEATHSTAR_PERIODIC_3] = "assets/sound/emote_death_star_periodic_3.ogg";
		soundList[Sound::EMOTE_DEATHSTAR_PERIODIC_4] = "assets/sound/emote_death_star_periodic_4.ogg";

		soundList[Sound::STORY_DEATHSTAR_INTRO] = "assets/sound/story_death_star_intro.ogg";
		soundList[Sound::STORY_TWINS_INTRO] = "assets/sound/story_stage1_intro.ogg";
		soundList[Sound::STORY_COUNCIL_INTRO] = "assets/sound/story_stage2_intro.ogg";
	}

	// String to Sound relation map
	{
		// ERROR MESSAGE
		musicStringList["null"] = ResourceHandler::Sound::FX_ERROR;

		// STORY
		musicStringList["stage3_intro_story"] = ResourceHandler::Sound::STORY_DEATHSTAR_INTRO;
		musicStringList["stage2_intro_story"] = ResourceHandler::Sound::STORY_COUNCIL_INTRO;
		musicStringList["stage1_intro_story"] = ResourceHandler::Sound::STORY_TWINS_INTRO;

		// EMOTES
		musicStringList["deathstar_rage"] = ResourceHandler::Sound::EMOTE_DEATHSTAR_GREET;
		musicStringList["deathstar_greet"] = ResourceHandler::Sound::EMOTE_DEATHSTAR_GREET;
		musicStringList["deathstar_periodic_1"] = ResourceHandler::Sound::EMOTE_DEATHSTAR_PERIODIC_1;
		musicStringList["deathstar_periodic_2"] = ResourceHandler::Sound::EMOTE_DEATHSTAR_PERIODIC_2;
		musicStringList["deathstar_periodic_3"] = ResourceHandler::Sound::EMOTE_DEATHSTAR_PERIODIC_3;
		musicStringList["deathstar_periodic_4"] = ResourceHandler::Sound::EMOTE_DEATHSTAR_PERIODIC_4;
		musicStringList["deathstar_death"] = ResourceHandler::Sound::EMOTE_DEATHSTAR_DEATH;
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

	// Message of the day 
	{
		messageOfTheDay.push_back("Its only a game!");
		messageOfTheDay.push_back("Spacesailor 3.0!");
		messageOfTheDay.push_back("Shooting everyday");
		messageOfTheDay.push_back("Bullets bullets bullets");
		messageOfTheDay.push_back("Dodging dem blue balls");
		messageOfTheDay.push_back("Green is the color!");
		messageOfTheDay.push_back("Dodgy dodgy!");
	}

	// Load each of the resources
	this->loadFonts();
	this->draw();
	this->loadTextures();
	this->loadSound();
	this->loadScripts();
	this->loadHighScore();
	this->loadUserName();
	this->loadCredits();

	// Set initialization to true.
	this->setInit(true);
}

/// <summary>
/// Loads the fonts.
/// </summary>
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


/// <summary>
/// Loads the textures.
/// </summary>
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

/// <summary>
/// Loads the sound.
/// </summary>
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

/// <summary>
/// Loads the high score.
/// </summary>
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

/// <summary>
/// Writes the high score score.
/// </summary>
/// <param name="score">The score.</param>
/// <param name="scriptEnum">The script enum.</param>
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

/// <summary>
/// Loads the scripts.
/// </summary>
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

				///////////////////////////////////////
				// Loop through each of the enemies////
				///////////////////////////////////////
				for (pugi::xml_node node : root.child("Enemies").children("Enemy"))
				{
					// Create a queue for the path
					std::queue<VectorN> pathQueue = std::queue<VectorN>();
					std::list<std::pair<int, std::string>> emoteQueue = std::list<std::pair<int, std::string>>();

					// Retrieve enemy type and delay data
					int eType = atoi(node.child("Type").child_value());
					int eDelay = atoi(node.child("Delay").child_value());

					// Retrieve each of the Paths
					for (pugi::xml_node path : node.child("Path").children())
					{
						// Convert string to integer
						int x = atoi(path.attribute("x").value());
						int y = atoi(path.attribute("y").value());
						int acceleration = atof(path.attribute("acceleration").value());
						int shoot = atoi(path.attribute("shoot").value());
						int sleep = atoi(path.attribute("sleep").value());
						std::string sId = path.attribute("sID").value();

						// Add to the "vector"
						VectorN vec;
						vec.x = x;
						vec.y = y;
						vec.shoot = shoot;
						vec.acceleration = acceleration;
						vec.sleepTime = sf::milliseconds(sleep);

						// See if the sId field is empty
						if (!sId.empty())
						{
							// Then check if the actual sId EXISTS
							if (musicStringList[sId] != 0)
							{
								vec.emote = std::shared_ptr<sf::Sound>(&this->getSound(musicStringList[sId]));
							}
							else
							{
								LOGE("Could not find a song record with: " << sId << " in the musicStringList!");
							}
						}
			
						// Push path into the queue
						pathQueue.push(vec);

					}

					// Retrieves each of the emotes
					for (pugi::xml_node emotes : node.child("Emotes").children())
					{
						// Get the percent and the emote name string
						std::string percent = emotes.attribute("percent").value();
						std::string sId = emotes.attribute("sID").value(); // ID of the sound (key to the emote map in resource handler)

						// Split on comma and append to the queue (percentages)
						std::stringstream ss(percent);
						int i;
						while (ss >> i)
						{
							// Create a pair to hold the percentage --> sound
							std::pair<int, std::string> par;
							par.first = i;
							par.second = sId;

							// Push the pair to the queue
							emoteQueue.push_back(par);

							// Next after ,
							if (ss.peek() == ',')
								ss.ignore();
						}
					}

					// Sort the emote queue
					emoteQueue.sort([](std::pair<int, std::string> & a, std::pair<int, std::string> & b) { return a.first > b.first; });

					// Add enemy to script
					this->scripts[i.first].addEnemy(eDelay, pathQueue, emoteQueue, eType, scriptRepeat);
					counter++;
				}
				LOGD(i.first << " was successfully loaded. " << counter << "enemies was queued.");

				///////////////////////////////////////
				// Loop through each of the power ups////
				///////////////////////////////////////
				counter = 0;
				for (pugi::xml_node node : root.child("PowerUps").children("PowerUp"))
				{

					// Retrieve powerup type and delay data
					int pwrType = atoi(node.child("Type").child_value());
					int pwrDelay = atoi(node.child("Delay").child_value());
					int pwrRepeat = atoi(node.child("Repeat").child_value());
					int pwrX = atoi(node.child("Path").attribute("x").value());
					int pwrY = atoi(node.child("Path").attribute("y").value());
					VectorN path;
					path.x = pwrX;
					path.y = pwrY;

					// Add powerup to script
					this->scripts[i.first].addPowerUp(pwrDelay, path, pwrType, pwrRepeat);
					counter++;
				}
				LOGD(i.first << " was successfully loaded. " << counter << "powerups was queued.");

				// Fetch Story
				pugi::xml_node story = root.child("Story");
				std::string storyIntro = "null";
				std::string lore = "null";
				if (!story.empty())
				{
					storyIntro = story.child("Intro").child_value();
					lore = story.child("Lore").child_value();
				}
				this->scripts[i.first].setLore(lore);
				this->scripts[i.first].setAudioDesc(storyIntro);


				// Set enum and Script title, then set status to initialized
				this->scripts[i.first].setScriptEnumVal(i.first);
				this->scripts[i.first].setScriptTitle(scriptName);
				this->scripts[i.first].setInit(true);


			}
			else
				// Handle a error message on exception
			{
				LOGD("Error description: " << result.description() << "\n");
			}
		}
	}
}

/// <summary>
/// Loads the name of the user. This is restricted to windows only
/// </summary>
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

/// <summary>
/// Loads the credits.
/// </summary>
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

/// <summary>
/// Gets the credits.
/// </summary>
/// <returns>Map with all credits</returns>
std::map<std::string, std::list<std::string>> ResourceHandler::getCredits()
{
	return creditsMap;
}

/// <summary>
/// Gets the date time.
/// </summary>
/// <returns>Returns current date</returns>
std::string ResourceHandler::getDateTime()
{
	std::chrono::system_clock::time_point p = std::chrono::system_clock::now();
	std::time_t t = std::chrono::system_clock::to_time_t(p);
	char buff[32];
	ctime_s(buff, sizeof(buff), &t);

	std::stringstream ss;
	ss << std::put_time(std::localtime(&t), "%y-%m-%d %X");
	return ss.str();
}

/// <summary>
/// Returns the player username (Computer name)
/// </summary>
/// <returns>The player's name</returns>
std::string ResourceHandler::getUserName()
{
	return this->userName;
}


/// <summary>
/// Gets the texture.
/// </summary>
/// <param name="res">The resource.</param>
/// <returns>Returns a texure reference</returns>
sf::Texture& ResourceHandler::getTexture(ResourceHandler::Texture res)
{
	return this->textures[res];
}

/// <summary>
/// Gets the script.
/// </summary>
/// <param name="query">The query.</param>
/// <returns>Returns a script copy</returns>
Script ResourceHandler::getScript(ResourceHandler::Scripts query)
{
	return this->scripts[query];
}

/// <summary>
/// Gets the script by an identifier. (Enum)
/// </summary>
/// <param name="iteNum">The ite number.</param>
/// <returns>A script</returns>
Script ResourceHandler::getScriptById(int iteNum)
{
	// Meh method
	int cnt = 1;
	for (Script& i : getScripts(true))
	{
		if (cnt == iteNum) return i;
		cnt++;
	}
}

/// <summary>
/// Gets all of the scripts.
/// </summary>
/// <returns>A list with scripts (all scripts)</returns>
std::list<Script> ResourceHandler::getScripts(bool encounterOnly)
{
	std::list<Script> ret;
	for (Script i : scripts)
	{
		if (i.getScriptEnumVal() != ResourceHandler::Scripts::GAME_MENU)
			ret.push_back(i);
	}
	return ret;
}

/// <summary>
/// Gets the font.
/// </summary>
/// <param name="query">The query.</param>
/// <returns>Font reference</returns>
sf::Font& ResourceHandler::getFont(ResourceHandler::Fonts query)
{
	return this->fonts[query];
}

/// <summary>
/// Gets the sound.
/// </summary>
/// <param name="query">The query.</param>
/// <returns>Sound reference</returns>
sf::Sound& ResourceHandler::getSound(ResourceHandler::Sound query)
{
	return this->sounds[query];
}

/// <summary>
/// Gets the sound by the emote name. This emote name origins from the ResourceHandler initial construction of the emoteMap
/// </summary>
/// <param name="emote">The emote name to use in order to retrieve the sound.</param>
/// <returns>Sound reference</returns>
sf::Sound& ResourceHandler::getSoundByEmoteName(std::string emote)
{
	if (emote == "null")
	{
		LOGE("Error: Missing emote/sound");
	}
	return this->sounds[musicStringList[emote]];
}

/// <summary>
/// Mutes all of the sound.
/// </summary>
/// <param name="mute">The mute variable (true/false)</param>
void ResourceHandler::muteSound(bool mute)
{
	for (auto& i : sounds)
	{
		(mute) ? i.setVolume(0) : i.setVolume(100);
	}
}


/// <summary>
/// Gets the high scores.
/// </summary>
/// <returns>map with all highscores</returns>
std::map<ResourceHandler::Scripts, std::list<std::shared_ptr<HighScoreItem>>> ResourceHandler::getHighScores()
{
	return this->highScoreStages;
}


/// <summary>
/// Gets the init value
/// </summary>
/// <returns>init flag</returns>
bool ResourceHandler::getInit()
{
	return this->inited;
}

/// <summary>
/// Sets the init flag
/// </summary>
/// <param name="init">The init flag</param>
void ResourceHandler::setInit(bool init)
{
	this->inited = init;
}

/// <summary>
/// Gets the message of the day.
/// </summary>
/// <param name="">Random id for message of the day</param>
/// <returns>The message of the day</returns>
std::string ResourceHandler::getMessageOfTheDay(int modt)
{
	return this->messageOfTheDay[modt];
}

/// <summary>
/// Gets the size of the motd vector
/// </summary>
/// <returns>Size of the MODT vector</returns>
int ResourceHandler::getMOTDSize()
{
	return this->messageOfTheDay.size();
}

/// <summary>
/// Draws the resource handler (loading state).
/// </summary>
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


