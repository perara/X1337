#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Constants.h"
#include "ScriptTemplate.h"
#include "HighScoreItem.h"
#include "Renderer.h"

#include <fstream>
#include <memory>
#include <map>
#include <list>



/// <summary>
/// The ResourceHandler loads all of the appropriate resources into the game. this is first used in World .
/// </summary>
class ResourceManager
{
public:

	ResourceManager(Renderer& window);
	~ResourceManager();

	void init();
	void draw();

	// Highscore
	std::map<Constants::ResourceC::Scripts, std::list<HighScoreItem>> getHighScores();
	void writeHighScoreScore(float, int);
	void loadHighScore();

	// Getts for each of the resources
	sf::Texture&getTextureByString(const std::string& str);
	sf::Texture& getTexture(Constants::ResourceC::Texture);

    std::unique_ptr<ScriptTemplate> getScript(Constants::ResourceC::Scripts);
	std::list<ScriptTemplate> getScripts(bool);

	sf::Font& getFont(Constants::ResourceC::Fonts);

	sf::Sound& getSound(Constants::ResourceC::Sound);
	sf::Sound& getSoundByEmoteName(const std::string&);

	void stopAllSound();
	void muteSound(bool mute);

	std::string getUserName();
	static std::string getDateTime();
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
	Renderer& window;

	// Textures
	std::map<std::string, Constants::ResourceC::Texture> textureStringList;
	std::map<Constants::ResourceC::Texture, std::string> textureList;
	sf::Texture textures[Constants::ResourceC::Texture::TEXTURECOUNT];

	// Sound
	std::map<Constants::ResourceC::Sound, std::string> soundList;
	sf::Sound sounds[Constants::ResourceC::SOUNDCOUNT];
	std::list<std::shared_ptr<sf::SoundBuffer>> sBufferList;

	// Emotes
	std::map<std::string, Constants::ResourceC::Sound> musicStringList;

	// Scripts
	std::map<Constants::ResourceC::Scripts, std::string> scriptList;
	ScriptTemplate scripts[Constants::ResourceC::Scripts::SCRIPTSCOUNT]; //TODO

	// Fonts
	std::map<Constants::ResourceC::Fonts, std::string> fontList;
	sf::Font fonts[Constants::ResourceC::Fonts::FONTCOUNT];

	// Highscore
	std::map<Constants::ResourceC::Scripts, std::list<HighScoreItem>> highScoreStages; // All stages
	std::string highScoreFile;

	// Username
	std::string userName;

	// Credits
	std::string creditsFilePath;
	std::map<std::string, std::list<std::string>> creditsMap;

	// Message of the day (below game title);
	std::vector<std::string> messageOfTheDay;
};
