#pragma once
#include <queue>
#include <list>
#include <SFML/Graphics.hpp>
#include <memory>
#include "Powerup.h"
#include "VectorN.h"
#include "Script.h"


class World;
class Shooter;
class BulletFactory;
class Bullet;
class ResourceManager;


/// <summary>
/// The script class defines a script loaded from file. It is widely used in the ResourceHandler
/// </summary>
class ScriptTemplate
{
public:
	std::list<ScriptTick> templateEnemies;
	std::list<ScriptTick> templatePowerups;

	ScriptTemplate():
	audioDesc("null")
	{};
	void addEnemy(int delay, std::queue<VectorN> pathQueue, std::list<std::pair<int, std::string>> emoteQueue, int type, int repeat);
	void addPowerUp(int delay, const VectorN& spawnPoint, int type, int repeat);

	/// Setter and getter for the title of the script (Defined in the xml file)
	std::string getScriptTitle();
	void setScriptTitle(std::string);

	/// Setter and getter for the enum value set in ResourceHandler
	void setScriptEnumVal(int);
	int getScriptEnumVal() const;

	/// Setter and getter for Audio description of the script
	void setAudioDesc(std::string);	
	std::string getAudioDesc();

	/// Setter and getter for the Lore string
	void setLore(std::string);
	std::string getLore();

	/// Sets and Gets the protrait String
	void setPortraitString(std::string);
	std::string getPortraitString();

    std::unique_ptr<Script> load(World* world);

private:
	std::string scriptTitle;
	std::string audioDesc;
	std::string lore;
	std::string portraitString;
	int scriptEnumVal;


};