#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "libs\rapidxml.hpp"
#include "libs\rapidxml_print.hpp"
#include <fstream>
#include <boost\lexical_cast.hpp>

#include "Script.h"

class ResourceHandler
{
public:
	const enum Texture{
		BACKGROUND1,
		TEXTURECOUNT
	};

	const enum Sound{
		SONG1,
		SOUNDCOUNT
	};
	const enum Scripts{
		ENCOUNTER1,
		ENCOUNTER2,
		ENCOUNTER3,
		SCRIPTSCOUNT
	};

	ResourceHandler(sf::RenderWindow& window);


	void init();
	void draw();
	void setInit(bool);
	bool getInit();


	sf::Texture* getTexture(ResourceHandler::Texture);
	Script* getScript(ResourceHandler::Scripts);

private:
	void loadTextures();
	void loadFont();
	void loadSound();
	void loadScripts();

	bool inited;
	sf::RenderWindow& window;

	// Textures
	std::map<Texture, std::string> textureList;
	sf::Texture textures[Texture::TEXTURECOUNT];

	// Sound
	std::map<Sound, std::string> soundList;
	sf::SoundBuffer sounds[Sound::SOUNDCOUNT];

	// Scripts
	std::map<Scripts, std::string> scriptList;
	Script scripts[Scripts::SCRIPTSCOUNT]; //TODO

	// Font
	sf::Font font;

};
