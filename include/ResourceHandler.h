#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#define PUGIXML_HEADER_ONLY
#include "pugixml.hpp"
#include "pugixml.cpp"
#include <fstream>
#include <memory>
#include <map>
#include <list>
#include "Script.h"

class HighScoreItem
{
public:
	int stage;
	std::string playerName;
	float score;
	std::string date;

	// Construct
	HighScoreItem(int stage, std::string playerName,
		float score,
		std::string date) : stage(stage), playerName(playerName), score(score), date(date){};
};

class ResourceHandler
{
public:
	const enum Texture
	{
		BACKGROUND1,
		BACKGROUND2,
		BACKGROUND3,
		HEART,
		MITT,
		BOSS,
		PLAYER_SHIP,
		ENEMY_SHIP,
		HEAVY_SHOT_TEXTURE,
		BOSS_DEATHSTAR_TEXTURE,
		CHUBBY_SHIP_TEXTURE,
		AUDIO_ON,
		AUDIO_OFF,
		PULSE_GUN,
		HEALTH_KIT,
		TEXTURECOUNT
	};

	const enum Sound
	{
		SONG1,
		MENU_SONG,
		STANDARD_SHOT,
		HEAVY_SHOT,
		ENEMY_DEATH,
		COUNTDOWN,
		INGAME,
		DEATH_STAR_THEME,
		PICKUP_HEALTH,
		MENU_CLICK,
		MENU_RETURN,
		SOUNDCOUNT

	};
	const enum Scripts
	{
		STAGE_ONE,
		STAGE_TWO,
		DEATH_STAR,
		TEST2,
		GAME_MENU,

		SCRIPTSCOUNT
	};

	const enum Fonts
	{
		COMICATE,
		SANSATION,
		FONTCOUNT,
	};

	ResourceHandler(sf::RenderWindow& window);
	~ResourceHandler();

	void init();
	void draw();

	// Highscore
	std::map<ResourceHandler::Scripts, std::list<std::shared_ptr<HighScoreItem>>> getHighScores();
	void writeHighScoreScore(int, int);
	void loadHighScore();

	// Getts for each of the resources
	sf::Texture& getTexture(ResourceHandler::Texture);
	Script getScript(ResourceHandler::Scripts);
	std::list<Script> getScripts();
	sf::Font& getFont(ResourceHandler::Fonts);
	sf::Sound& getSound(ResourceHandler::Sound);
	void muteSound(bool mute);
	Script getScriptById(int iteNum);
	std::string getUserName();
	std::string getDateTime();
	std::map<std::string, std::list<std::string>>  getCredits();

private:
	void loadTextures();
	void loadFonts();
	void loadSound();
	void loadScripts();
	void loadUserName();
	void loadCredits();

	void setInit(bool);
	bool getInit();

	bool inited;
	sf::RenderWindow& window;

	// Textures
	std::map<Texture, std::string> textureList;
	sf::Texture textures[Texture::TEXTURECOUNT];

	// Sound
	std::map<Sound, std::string> soundList;
	sf::Sound sounds[Sound::SOUNDCOUNT];
	std::list<std::shared_ptr<sf::SoundBuffer>> sBufferList;

	// Scripts
	std::map<Scripts, std::string> scriptList;
	Script scripts[Scripts::SCRIPTSCOUNT]; //TODO

	// Fonts
	std::map<Fonts, std::string> fontList;
	sf::Font fonts[Fonts::FONTCOUNT];

	// Highscore
	std::map<ResourceHandler::Scripts, std::list<std::shared_ptr<HighScoreItem>>> highScoreStages; // All stages
	std::string highScoreFile;

	// Username
	std::string userName;

	// Credits
	std::string creditsFilePath;
	std::map<std::string, std::list<std::string>> creditsMap;
};
