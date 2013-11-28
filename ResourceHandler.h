#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "libs\rapidxml.hpp"
#include "libs\rapidxml_print.hpp"
#include <fstream>
#include <memory>

#include "Script.h"

class ResourceHandler
{
public:
	const enum Texture
	{
		BACKGROUND1,
		HEART,
		TEXTURECOUNT
	};

	const enum Sound
	{
		SONG1,
		MENU_SONG,
		SOUNDCOUNT
	};
	const enum Scripts
	{
		ENCOUNTER1,
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


	sf::Texture& getTexture(ResourceHandler::Texture);
	Script getScript(ResourceHandler::Scripts);
	std::list<Script> getScripts();
	sf::Font& getFont(ResourceHandler::Fonts);
	sf::Sound& getSound(ResourceHandler::Sound);
	Script getScriptById(int iteNum);

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
};
