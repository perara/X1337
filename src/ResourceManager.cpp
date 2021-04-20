#include "../include/ResourceManager.h"
#include "../include/Log.h"
#include "../include/Enemy.h"
#include <spdlog/spdlog.h>
#include <memory>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip> // put_time
#include <pugixml.hpp>

#include <filesystem>
#include <cmrc/cmrc.hpp>
#include <nlohmann/json.hpp>

CMRC_DECLARE(xleetrc);



/// <summary>
/// Initializes a new instance of the <see cref="ResourceHandler"/> class.
/// </summary>
/// <param name="window">The window.</param>
ResourceManager::ResourceManager(Renderer& window) :
inited(false),
window(window)
{
}

/// <summary>
/// Finalizes an instance of the <see cref="ResourceHandler"/> class.
/// </summary>
ResourceManager::~ResourceManager()
= default;

/// <summary>
/// Initializes the resource handler.
/// </summary>
void ResourceManager::init()
{
	// Define Resources
	// HIGHSCORE
	creditsFilePath = "assets/credits.json";
	highScoreFile = "assets/state.json";

	// Textures
	{
		textureList[Constants::ResourceC::Texture::NOID] = "assets/sprites/noid.png";
		textureList[Constants::ResourceC::Texture::BACKGROUND2] = "assets/sprites/bg2.png";
		textureList[Constants::ResourceC::Texture::BACKGROUND3] = "assets/sprites/bg3.png";
		textureList[Constants::ResourceC::Texture::HEART] = "assets/sprites/heart.png";
		textureList[Constants::ResourceC::Texture::PLAYER_SHIP] = "assets/sprites/player_ship.png";
		textureList[Constants::ResourceC::Texture::ENEMY_SHIP] = "assets/sprites/enemy_ship.png";
		textureList[Constants::ResourceC::Texture::BOSS] = "assets/sprites/boss.png";
		textureList[Constants::ResourceC::Texture::HEAVY_SHOT_TEXTURE] = "assets/sprites/heavy_shot.png";
		textureList[Constants::ResourceC::Texture::REGULAR_BULLET_1] = "assets/sprites/regular_bullet_1.png";
		textureList[Constants::ResourceC::Texture::BOSS_DEATHSTAR_TEXTURE] = "assets/sprites/deathstar.png";
		textureList[Constants::ResourceC::Texture::CHUBBY_SHIP_TEXTURE] = "assets/sprites/chubby.png";

		textureList[Constants::ResourceC::Texture::AUDIO_ON] = "assets/sprites/audio-on.png";
		textureList[Constants::ResourceC::Texture::AUDIO_OFF] = "assets/sprites/audio-off.png";
		textureList[Constants::ResourceC::Texture::MONITOR_ICON] = "assets/sprites/monitor.png";
		textureList[Constants::ResourceC::Texture::HEALTH_KIT] = "assets/sprites/health-kit.png";
		textureList[Constants::ResourceC::Texture::PULSE_GUN] = "assets/sprites/pulse-gun.png";
		textureList[Constants::ResourceC::Texture::PLAYER_BAR] = "assets/sprites/player-bar.png";

		textureList[Constants::ResourceC::Texture::PORTRAIT_TWINS] = "assets/sprites/stage_portraits/twins.png";
		textureList[Constants::ResourceC::Texture::PORTRAIT_COUNCIL] = "assets/sprites/stage_portraits/council.png";
		textureList[Constants::ResourceC::Texture::PORTRAIT_DEATHSTAR] = "assets/sprites/stage_portraits/deathstar.png";
	}

	// Sounds
	{
		soundList[Constants::ResourceC::Sound::MUSIC_MENU_SONG] = "assets/sound/game_menu.ogg";
		soundList[Constants::ResourceC::Sound::FX_STANDARD_SHOT] = "assets/sound/FireOneSound.ogg";
		soundList[Constants::ResourceC::Sound::FX_HEAVY_SHOT] = "assets/sound/FireOneSound.ogg";
		soundList[Constants::ResourceC::Sound::FX_ENEMY_DEATH] = "assets/sound/ExplosionSound.ogg";
		soundList[Constants::ResourceC::Sound::MUSIC_INGAME] = "assets/sound/in-game.ogg";
		soundList[Constants::ResourceC::Sound::MUSIC_COUNTDOWN] = "assets/sound/countdown.ogg";
		soundList[Constants::ResourceC::Sound::MUSIC_DEATH_STAR_THEME] = "assets/sound/death_star_theme.ogg";
		soundList[Constants::ResourceC::Sound::FX_PICKUP_HEALTH] = "assets/sound/health_pickup.wav";
		soundList[Constants::ResourceC::Sound::FX_MENU_CLICK] = "assets/sound/menu_click.ogg";
		soundList[Constants::ResourceC::Sound::FX_MENU_RETURN] = "assets/sound/menu_return.ogg";
		soundList[Constants::ResourceC::Sound::FX_ERROR] = "assets/sound/fx_error.wav";
		soundList[Constants::ResourceC::Sound::FX_BOUNCE] = "assets/sound/fx_bounce.ogg";

		soundList[Constants::ResourceC::Sound::EMOTE_DEATHSTAR_GREET] = "assets/sound/emote_death_star_greet.ogg";
		soundList[Constants::ResourceC::Sound::EMOTE_DEATHSTAR_BEWARE] = "assets/sound/emote_death_star_beware_cowards.ogg";
		soundList[Constants::ResourceC::Sound::EMOTE_DEATHSTAR_DEATH] = "assets/sound/emote_death_star_enough.ogg";
		soundList[Constants::ResourceC::Sound::EMOTE_DEATHSTAR_PERIODIC_1] = "assets/sound/emote_death_star_periodic_1.ogg";
		soundList[Constants::ResourceC::Sound::EMOTE_DEATHSTAR_PERIODIC_2] = "assets/sound/emote_death_star_periodic_2.ogg";
		soundList[Constants::ResourceC::Sound::EMOTE_DEATHSTAR_PERIODIC_3] = "assets/sound/emote_death_star_periodic_3.ogg";
		soundList[Constants::ResourceC::Sound::EMOTE_DEATHSTAR_PERIODIC_4] = "assets/sound/emote_death_star_periodic_4.ogg";

		soundList[Constants::ResourceC::Sound::STORY_DEATHSTAR_INTRO] = "assets/sound/story_death_star_intro.ogg";
		soundList[Constants::ResourceC::Sound::STORY_TWINS_INTRO] = "assets/sound/story_stage1_intro.ogg";
		soundList[Constants::ResourceC::Sound::STORY_COUNCIL_INTRO] = "assets/sound/story_stage2_intro.ogg";
	}

	// String to Sound relation map
	{
		// ERROR MESSAGE
		musicStringList["null"] = Constants::ResourceC::Sound::FX_ERROR;

		// STORY
		musicStringList["stage3_intro_story"] = Constants::ResourceC::Sound::STORY_DEATHSTAR_INTRO;
		musicStringList["stage2_intro_story"] = Constants::ResourceC::Sound::STORY_COUNCIL_INTRO;
		musicStringList["stage1_intro_story"] = Constants::ResourceC::Sound::STORY_TWINS_INTRO;

		// EMOTES
		musicStringList["deathstar_greet"] = Constants::ResourceC::Sound::EMOTE_DEATHSTAR_GREET;
		musicStringList["deathstar_periodic_1"] = Constants::ResourceC::Sound::EMOTE_DEATHSTAR_PERIODIC_1;
		musicStringList["deathstar_periodic_2"] = Constants::ResourceC::Sound::EMOTE_DEATHSTAR_PERIODIC_2;
		musicStringList["deathstar_periodic_3"] = Constants::ResourceC::Sound::EMOTE_DEATHSTAR_PERIODIC_3;
		musicStringList["deathstar_periodic_4"] = Constants::ResourceC::Sound::EMOTE_DEATHSTAR_PERIODIC_4;
		musicStringList["deathstar_death"] = Constants::ResourceC::Sound::EMOTE_DEATHSTAR_DEATH;
		musicStringList["deathstar_rage"] = Constants::ResourceC::Sound::EMOTE_DEATHSTAR_DEATH;
		musicStringList["deathstar_beware"] = Constants::ResourceC::Sound::EMOTE_DEATHSTAR_BEWARE;
	}

	// Texture string to texture map
	{
		textureStringList["deathstar"] = Constants::ResourceC::Texture::PORTRAIT_DEATHSTAR;
		textureStringList["council"] = Constants::ResourceC::Texture::PORTRAIT_COUNCIL;
		textureStringList["twins"] = Constants::ResourceC::Texture::PORTRAIT_TWINS;

	}


	// Scripts
	{
		scriptList[Constants::ResourceC::Scripts::STAGE_ONE] = "assets/scripts/stage1.xml";
		scriptList[Constants::ResourceC::Scripts::STAGE_TWO] = "assets/scripts/stage2.xml";
		scriptList[Constants::ResourceC::Scripts::GAME_MENU] = "assets/scripts/game_menu.xml";
		scriptList[Constants::ResourceC::Scripts::DEATH_STAR] = "assets/scripts/test1.xml";
	}

	// Fonts
	{
		fontList[Constants::ResourceC::Fonts::COMICATE] = "assets/fonts/comicate.ttf";
		fontList[Constants::ResourceC::Fonts::SANSATION] = "assets/fonts/sansation.ttf";
	}

	// Message of the day
	{
		messageOfTheDay.emplace_back("Its only a game!");
		messageOfTheDay.emplace_back("Spacesailor 3.0!");
		messageOfTheDay.emplace_back("Shooting everyday");
		messageOfTheDay.emplace_back("Bullets bullets bullets");
		messageOfTheDay.emplace_back("Dodging dem blue balls");
		messageOfTheDay.emplace_back("Green is the color!");
		messageOfTheDay.emplace_back("Dodgy dodgy!");
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
void ResourceManager::loadFonts()
{
    auto fs = cmrc::xleetrc::get_filesystem();

	// Load Fonts
	for (auto& i : fontList)
	{
        auto file = fs.open(i.second);
        auto data = std::string_view(file.begin(), file.end() - file.begin());
		if (fonts[i.first].loadFromMemory(data.data(), data.size())){
            SPDLOG_INFO("Font loaded: {}", i.second);
		}
		else
		{
            SPDLOG_INFO("Failed to load font: {}", i.second);

		}
	}
}


/// <summary>
/// Loads the textures.
/// </summary>
void ResourceManager::loadTextures()
{
    auto fs = cmrc::xleetrc::get_filesystem();

	// Load Textures
	for (auto& i : textureList)
	{
        auto file = fs.open(i.second);
        auto data = std::string_view(file.begin(), file.end() - file.begin());
		if (textures[i.first].loadFromMemory(data.data(), data.size())){
            SPDLOG_INFO("Texture loaded: {}", i.second);
		}
		else
		{
            SPDLOG_INFO("Failed to load texture: {}", i.second);

		}
	}
}

/// <summary>
/// Loads the sound.
/// </summary>
void ResourceManager::loadSound()
{
    auto fs = cmrc::xleetrc::get_filesystem();
	// Load sounds
	for (auto& i : soundList)
	{
        auto file = fs.open(i.second);
        auto data = std::string_view(file.begin(), file.end() - file.begin());
		std::shared_ptr<sf::SoundBuffer> buf = std::make_shared<sf::SoundBuffer>();
		sBufferList.push_back(buf);
		if (buf->loadFromMemory(data.data(), data.size())){
			sounds[i.first].setBuffer(*buf);
            SPDLOG_INFO("Sound loaded: {}", i.second);
		}
		else
		{
            SPDLOG_INFO("Failed to load sound: {}", i.second);
		}

	}
}

/// <summary>
/// Loads the high score.
/// </summary>

// TODO
// /mnt/disk1/From_Nas/to_move/nextcloud_data/per/files/Documents/University of Agder/DAT-220/Project/source/Script Maker
void ResourceManager::loadHighScore()
{
    auto stateFile = std::filesystem::current_path().append("state.json");

    if(!exists(stateFile)){
        auto fs = cmrc::xleetrc::get_filesystem();
        auto file = fs.open(highScoreFile);
        auto data = std::string_view(file.begin(), file.end() - file.begin());
        std::ofstream(stateFile) << data;
    }


	// Read XML file raw (Create file Stream)
	std::ifstream fileStream(stateFile);

	if (fileStream.good())
	{
		auto data = nlohmann::json::parse(fileStream);

		for(auto &stage : data["stages"]){

            // Get the stage enum from attribute
            int stageID = stage["id"].get<int>();

            for(auto &playerItem : stage["players"]){
                auto playerName = playerItem["name"].get<std::string>();
                auto playerScore = playerItem["score"].get<float>();
                auto playerDate = playerItem["date"].get<std::string>();

                highScoreStages[(Constants::ResourceC::Scripts)stageID].emplace_back(
                        (Constants::ResourceC::Scripts)stageID,
                        playerName,
                        playerScore,
                        playerDate
                );
            }

            // Sort by score (ASC) then add to highScoreMap
            highScoreStages[(Constants::ResourceC::Scripts)stageID]
            .sort([](HighScoreItem & a, HighScoreItem & b) { return a.score > b.score; });

		}
	}
}

/// <summary>
/// Writes the high score score.
/// </summary>
/// <param name="score">The score.</param>
/// <param name="scriptEnum">The script enum.</param>
void ResourceManager::writeHighScoreScore(float score, int scriptEnum)
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
            SPDLOG_INFO("Error description: {}\n", result.description());
		}
	}
	loadHighScore(); // RELOAD  XML
}

/// <summary>
/// Loads the scripts.
/// </summary>
void ResourceManager::loadScripts()
{
    auto fs = cmrc::xleetrc::get_filesystem();


	// Load Scripts
	for (auto& i : scriptList)
	{
		// Read XML file raw (Create file Stream)
        auto file = fs.open(i.second);


		if (file.size())
		{
            auto data = std::string_view(file.begin(), file.end() - file.begin());

			// Load XML and parse it
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_string(data.data());

			// If XML was parsed successfully without errors
			if (result)
			{
				// Set the root node
				pugi::xml_node root = doc.child("Script");

				int scriptRepeat = atoi(root.child("Repeat").child_value());
				std::string scriptName = root.child("Name").child_value();
				std::string portrait = root.child("Portrait").child_value();

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
                    float eType = std::strtof(node.child("Type").child_value(), nullptr);
                    float eDelay = std::strtof(node.child("Delay").child_value(), nullptr);

					// Retrieve each of the Paths
					for (pugi::xml_node path : node.child("Path").children())
					{
						// Convert string to integer
                        float x = std::strtof(path.attribute("x").value(), nullptr);
                        float y = std::strtof(path.attribute("y").value(), nullptr);
                        float acceleration = std::strtof(path.attribute("acceleration").value(), nullptr);
                        float shoot = std::strtof(path.attribute("shoot").value(), nullptr);
                        float sleep = std::strtof(path.attribute("sleep").value(), nullptr);
						std::string sId = path.attribute("sID").value();

						// Add to the "vector"
						VectorN vec;
						vec.x = x;
						vec.y = y;
						vec.shoot = (int)shoot;
						vec.acceleration = acceleration;
						vec.sleepTime = sf::milliseconds((int)sleep);

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
						int _i;
						while (ss >> _i)
						{
							// Create a pair to hold the percentage --> sound
							std::pair<int, std::string> par;
							par.first = _i;
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
					this->scripts[i.first].addEnemy(
                            (int)eDelay,
					        pathQueue,
					        emoteQueue,
                            (int)eType,
					        scriptRepeat);
					counter++;
				}
                SPDLOG_INFO( "{} was successfully loaded. {} enemies was queued.", i.first, counter);

				///////////////////////////////////////
				// Loop through each of the power ups////
				///////////////////////////////////////
				counter = 0;
				for (pugi::xml_node node : root.child("PowerUps").children("PowerUp"))
				{

					// Retrieve powerup type and delay data
					float pwrType = std::strtof(node.child("Type").child_value(), nullptr);
                    float pwrDelay = std::strtof(node.child("Delay").child_value(), nullptr);
                    float pwrRepeat = std::strtof(node.child("Repeat").child_value(), nullptr);
                    float pwrX = std::strtof(node.child("Path").attribute("x").value(), nullptr);
                    float pwrY = std::strtof(node.child("Path").attribute("y").value(), nullptr);
					VectorN path;
					path.x = pwrX;
					path.y = pwrY;

					// Add powerup to script
					this->scripts[i.first].addPowerUp(
                            (int)pwrDelay,
					        path,
                            (int)pwrType,
                            (int)pwrRepeat);
					counter++;
				}
                SPDLOG_INFO("{} was successfully loaded. {} powerups was queued.", i.first, counter);

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
				this->scripts[i.first].setPortraitString(portrait);


			}
			else
				// Handle a error message on exception
			{
                SPDLOG_INFO("Error description:", result.description());
			}
		}
	}
}

/// <summary>
/// Loads the name of the user.
/// </summary>
void ResourceManager::loadUserName()
{
	this->userName = "Player";
}

/// <summary>
/// Loads the credits.
/// </summary>
void ResourceManager::loadCredits()
{
    auto fs = cmrc::xleetrc::get_filesystem();
	if (!fs.exists(creditsFilePath)) {
        SPDLOG_INFO("Credits file could not be found!: {}");
    }
    auto file = fs.open(creditsFilePath);

	auto data = nlohmann::json::parse(file.begin(), file.end());

	for(const auto& item : data.items()) {

        for (const auto &node : item.value()) {

            // Add the list to map index.
            creditsMap[item.key()].emplace_back(node.get<std::string>());
        }
    }

}

/// <summary>
/// Gets the credits.
/// </summary>
/// <returns>Map with all credits</returns>
std::map<std::string, std::list<std::string>> ResourceManager::getCredits()
{
	return creditsMap;
}

/// <summary>
/// Gets the date time.
/// </summary>
/// <returns>Returns current date</returns>
std::string ResourceManager::getDateTime()
{
	std::chrono::system_clock::time_point p = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(p);

	std::stringstream ss;
	ss << std::put_time(std::localtime(&t), "%y-%m-%d %X");
	return ss.str();
}

/// <summary>
/// Returns the player username (Computer name)
/// </summary>
/// <returns>The player's name</returns>
std::string ResourceManager::getUserName()
{
	return this->userName;
}


/// <summary>
/// Gets the texture.
/// </summary>
/// <param name="res">The resource.</param>
/// <returns>Returns a texure reference</returns>
sf::Texture& ResourceManager::getTexture(Constants::ResourceC::Texture res)
{
	return this->textures[res];
}

/// <summary>
/// Gets the script.
/// </summary>
/// <param name="query">The query.</param>
/// <returns>Returns a script copy</returns>
std::unique_ptr<ScriptTemplate> ResourceManager::getScript(Constants::ResourceC::Scripts scriptID)
{
	return std::make_unique<ScriptTemplate>(this->scripts[scriptID]);
}


/// <summary>
/// Gets all of the scripts.
/// </summary>
/// <returns>A list with scripts (all scripts)</returns>
std::list<ScriptTemplate> ResourceManager::getScripts(bool encounterOnly)
{
	std::list<ScriptTemplate> ret;
	for (ScriptTemplate i : scripts)
	{
		if (i.getScriptEnumVal() != Constants::ResourceC::Scripts::GAME_MENU)
			ret.push_back(i);
	}
	return ret;
}

/// <summary>
/// Gets the font.
/// </summary>
/// <param name="query">The query.</param>
/// <returns>Font reference</returns>
sf::Font& ResourceManager::getFont(Constants::ResourceC::Fonts query)
{
	return this->fonts[query];
}

/// <summary>
/// Gets the sound.
/// </summary>
/// <param name="query">The query.</param>
/// <returns>Sound reference</returns>
sf::Sound& ResourceManager::getSound(Constants::ResourceC::Sound query)
{
	return this->sounds[query];
}

/// <summary>
/// Gets the sound by the emote name. This emote name origins from the ResourceHandler initial construction of the emoteMap
/// </summary>
/// <param name="emote">The emote name to use in order to retrieve the sound.</param>
/// <returns>Sound reference</returns>
sf::Sound& ResourceManager::getSoundByEmoteName(const std::string& emote)
{
	if (emote == "null")
	{
		LOGE("Error: Missing emote/sound");
	}
	return this->sounds[musicStringList[emote]];
}

/// <summary>
/// Gets the texture by string (Via the string to resource texture map)
/// </summary>
/// <param name="str">The string.</param>
/// <returns>A texture reference</returns>
sf::Texture& ResourceManager::getTextureByString(const std::string& str)
{
	return textures[textureStringList[str]];
}

/// <summary>
/// Mutes all of the sound.
/// </summary>
/// <param name="mute">The mute variable (true/false)</param>
void ResourceManager::muteSound(bool mute)
{
	for (auto& i : sounds)
	{
		(mute) ? i.setVolume(0) : i.setVolume(100);
	}
}

/// <summary>
/// Mutes all of the sound.
/// </summary>
/// <param name="mute">The mute variable (true/false)</param>
void ResourceManager::stopAllSound()
{
	for (auto& i : sounds)
	{
		i.stop();
	}
}



/// <summary>
/// Gets the high scores.
/// </summary>
/// <returns>map with all highscores</returns>
std::map<Constants::ResourceC::Scripts, std::list<HighScoreItem>> ResourceManager::getHighScores()
{
	return this->highScoreStages;
}


/// <summary>
/// Gets the init value
/// </summary>
/// <returns>init flag</returns>
bool ResourceManager::getInit()
{
	return this->inited;
}

/// <summary>
/// Sets the init flag
/// </summary>
/// <param name="init">The init flag</param>
void ResourceManager::setInit(bool _init)
{
	this->inited = _init;
}

/// <summary>
/// Gets the message of the day.
/// </summary>
/// <param name="">Random id for message of the day</param>
/// <returns>The message of the day</returns>
std::string ResourceManager::getMessageOfTheDay(int modt)
{
	return this->messageOfTheDay[modt];
}

/// <summary>
/// Gets the size of the motd vector
/// </summary>
/// <returns>Size of the MODT vector</returns>
int ResourceManager::getMOTDSize()
{
	return this->messageOfTheDay.size();
}

/// <summary>
/// Draws the resource handler (loading state).
/// </summary>
void ResourceManager::draw()
{
	sf::Text label;
	label.setFont(this->getFont(Constants::ResourceC::Fonts::COMICATE));
	label.setString(sf::String("Loading... Please Wait!"));
	label.setPosition(
		window.getView().getCenter().x - (label.getGlobalBounds().width / 2),
		window.getView().getCenter().y / 2 - (label.getGlobalBounds().height / 2));
	label.setFillColor(sf::Color::White);


	window.clear(sf::Color::Black);
	window.draw(label);
	window.display();



}


