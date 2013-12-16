#pragma once
#include <queue>
#include <list>
#include <SFML\Graphics.hpp>
#include <memory>
#include "Powerup.h"
#include "VectorN.h"

class Shooter;
class BulletFactory;
class Bullet;
class ResourceHandler;

/// <summary>
/// The script class defines a script loaded from file. It is widely used in the ResourceHandler
/// </summary>
class Script
{
	class ScriptTick
	{
	public:
		int delay;
		int repeat;
		int type;
		std::queue<VectorN> pathQueue;
		std::list<std::pair<int, std::string>> emoteQueue;

		ScriptTick(int delay, std::queue<VectorN> pathQueue, std::list<std::pair<int, std::string>> emoteQueue, int type, int repeat) :
			delay(delay),
			pathQueue(pathQueue),
			emoteQueue(emoteQueue),
			type(type),
			repeat(repeat)
			{};
	};

	sf::Time enemyTime;
	sf::Time powerupTime;
	std::queue<ScriptTick> enemyList;
	std::queue<ScriptTick> powerupList;
	bool inited;


public:
	Script():
	audioDesc("null"),
	startEnemyListSize(-1)
	{};
	void addEnemy(int delay, std::queue<VectorN> pathQueue, std::list<std::pair<int, std::string>> emoteQueue, int type, int repeat);
	void addPowerUp(int delay, VectorN spawnPoint, int type, int repeat);

	// Init
	bool getInit();
	void setInit(bool);

	// Process
	bool process(sf::RenderWindow& window,
		std::list<std::shared_ptr<Shooter>>& objects,
		std::list<std::shared_ptr<Powerup>>& powerups,
		std::list<std::unique_ptr<Bullet>>& bullets,
		BulletFactory& bFactory,
		std::shared_ptr<ResourceHandler>& resourceHandler,
		const sf::Time& timeStep);

	/// Setter and getter for the title of the script (Defined in the xml file)
	std::string getScriptTitle();
	void setScriptTitle(std::string);

	/// Setter and getter for the enum value set in ResourceHandler
	void setScriptEnumVal(int);
	int getScriptEnumVal();

	/// Setter and getter for Audio description of the script
	void setAudioDesc(std::string);	
	std::string getAudioDesc();

	/// Setter and getter for the Lore string
	void setLore(std::string);
	std::string getLore();

	///  Get for enemy list size (start and current)
	const int getStartEnemyListSize();
	const int getEnemyListSize();

private:
	std::string scriptTitle;
	std::string audioDesc;
	std::string lore;
	int scriptEnumVal;
	int startEnemyListSize;

};