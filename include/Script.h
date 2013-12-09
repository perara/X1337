#pragma once
#include <queue>
#include <list>
#include <SFML\Graphics.hpp>
#include <memory>
#include "Powerup.h"


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
		std::queue<sf::Vector3f> pathQueue;

		ScriptTick(int delay, std::queue<sf::Vector3f> pathQueue, int type, int repeat) :
			delay(delay),
			pathQueue(pathQueue),
			type(type),
			repeat(repeat){};
	};


	sf::Clock enemyClock;
	sf::Clock powerupClock;
	std::queue<ScriptTick> enemyList;
	std::queue<ScriptTick> powerupList;
	bool inited;


public:
	Script(){};
	void addEnemy(int delay, std::queue<sf::Vector3f> pathQueue, int type, int repeat);
	void addPowerUp(int delay, sf::Vector3f spawnPoint, int type, int repeat);

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

	// Get/Set scriptName
	std::string getScriptTitle();
	void setScriptTitle(std::string);
	void setScriptEnumVal(int);
	int getScriptEnumVal();
private:
	std::string scriptTitle;
	int scriptEnumVal;

};