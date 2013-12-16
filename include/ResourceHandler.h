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

/// <summary>
/// This class keeps a record a highscore item, this is used in the ResourceHandler
/// </summary>
class HighScoreItem
{
public:
	/// Integer which describe the stage this HighScoreItem belongs to
	int stage;

	/// Contains the Playername for current HighScoreItem
	std::string playerName;

	/// Contains the score achieved for this HighScoreItem
	float score;

	/// Contains the date for when this HighScoreItem was obtained (Written in ResourceHandler)
	std::string date;

	/// Constructor
	HighScoreItem(int stage, std::string playerName,
		float score,
		std::string date) : stage(stage), playerName(playerName), score(score), date(date){};
};

/// <summary>
/// The ResourceHandler loads all of the appropriate resources into the game. this is first used in World .
/// </summary>
class ResourceHandler
{
public:

	/// Texture enumerable which contains all resource "Links" its used to access the resource
	const enum Texture
	{
		BACKGROUND2,
		BACKGROUND3,
		HEART,
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
		REGULAR_BULLET_1,
		PLAYER_BAR,

		TEXTURECOUNT
	};

	/// Sound enumerable which contains all resource "Links" its used to access the resource
	const enum Sound
	{
		MUSIC_DEATH_STAR_THEME = 1,
		MUSIC_MENU_SONG,
		MUSIC_COUNTDOWN,
		MUSIC_INGAME, // The usual ingame for all stages

		FX_STANDARD_SHOT,
		FX_HEAVY_SHOT,
		FX_ENEMY_DEATH,
		FX_PICKUP_HEALTH,
		FX_MENU_CLICK,
		FX_MENU_RETURN,
		FX_ERROR,
		FX_BOUNCE,

		EMOTE_DEATHSTAR_GREET,
		EMOTE_DEATHSTAR_BEWARE,
		EMOTE_DEATHSTAR_PERIODIC_1,
		EMOTE_DEATHSTAR_PERIODIC_2,
		EMOTE_DEATHSTAR_PERIODIC_3,
		EMOTE_DEATHSTAR_PERIODIC_4,
		EMOTE_DEATHSTAR_DEATH,

		STORY_DEATHSTAR_INTRO,
		STORY_TWINS_INTRO,
		STORY_COUNCIL_INTRO,

		SOUNDCOUNT

	};

	/// Scripts enumerable which contains all resource "Links" its used to access the resource
	const enum Scripts
	{
		STAGE_ONE,
		STAGE_TWO,
		DEATH_STAR,
		TEST2,
		GAME_MENU,

		SCRIPTSCOUNT
	};

	/// Fonts enumerable which contains all resource "Links" its used to access the resource
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
	std::list<Script> getScripts(bool);
	sf::Font& getFont(ResourceHandler::Fonts);
	sf::Sound& getSound(ResourceHandler::Sound);
	sf::Sound& getSoundByEmoteName(std::string);

	void stopAllSound();
	void muteSound(bool mute);
	Script getScriptById(int iteNum);
	std::string getUserName();
	std::string getDateTime();
	std::map<std::string, std::list<std::string>>  getCredits();

	std::string getMessageOfTheDay(int);
	int getMOTDSize();


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
	sf::Sound sounds[ResourceHandler::SOUNDCOUNT];
	std::list<std::shared_ptr<sf::SoundBuffer>> sBufferList;

	// Emotes
	std::map<std::string, ResourceHandler::Sound> musicStringList;

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

	// Message of the day (below game title);
	std::vector<std::string> messageOfTheDay;
};
