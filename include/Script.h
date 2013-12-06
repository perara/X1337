#pragma once
#include <queue>
#include <list>
#include <SFML\Graphics.hpp>
#include <memory>


class Shooter;
class BulletFactory;
class Bullet;
class ResourceHandler;

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

	sf::Clock scriptClock;
	std::queue<ScriptTick> enemyList;
	bool inited;


public:
	Script(){};
	void addEnemy(int delay, std::queue<sf::Vector3f> pathQueue, int type, int repeat);

	// ScriptClock
	sf::Clock& getClock();
	void setClock(sf::Clock&);

	// Init
	bool getInit();
	void setInit(bool);

	// Process
	bool process(sf::RenderWindow& window,
		std::list<std::shared_ptr<Shooter>>& objects,
		std::list<std::unique_ptr<Bullet>>& bullets,
		BulletFactory& bFactory,
		std::unique_ptr<ResourceHandler>& resourceHandler,
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