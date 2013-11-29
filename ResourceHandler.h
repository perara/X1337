#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "libs\rapidxml.hpp"
#include "libs\rapidxml_print.hpp"
#include <fstream>
#include <memory>
#include <map>
#include <list>
#include "Script.h"

struct HighScoreItem
{
	int stage;
	std::string playerName;
	float score;
	std::string date;

	// Construct
	HighScoreItem(int stage, std::string playerName, 
		float score, 
		std::string date): stage(stage), playerName(playerName), score(score), date(date){};
};

class ResourceHandler
{
public:
	const enum Texture
	{
		BACKGROUND1,
		HEART,
		MITT,
		TEXTURECOUNT
	};

	const enum Sound
	{
		SONG1,
		MENU_SONG,
		STANDARD_SHOT,
		HEAVY_SHOT,
		ENEMY_DEATH,
		INGAME,
		SOUNDCOUNT

	};
	const enum Scripts
	{
		ENCOUNTER1,
		STAGEONE,
		ENCOUNTER2,
		ENCOUNTER3,
		GAME_MENU,
		DRIT,
		DRIT1,
		DRIT2,
		DRIT3,
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
	void setInit(bool);
	bool getInit();

	// Highscore
	void writeHighScoreScore();
	void loadHighScore();

	sf::Texture& getTexture(ResourceHandler::Texture);
	Script getScript(ResourceHandler::Scripts);
	std::list<Script> getScripts();
	sf::Font& getFont(ResourceHandler::Fonts);
	sf::Sound& getSound(ResourceHandler::Sound);
	Script getScriptById(int iteNum);
	std::map<ResourceHandler::Scripts, std::list<std::shared_ptr<HighScoreItem>>> getHighScores();

private:
	void loadTextures();
	void loadFonts();
	void loadSound();
	void loadScripts();

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
};
