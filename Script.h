#pragma once
#include <queue>
#include <list>
#include <SFML\System\Clock.hpp>


class Object;
class Shooter;
class Enemy;


class Script
{
	class ScriptTick
	{
	public:
		Enemy* enemy;
		int delay;
		ScriptTick(Enemy* enemy, int delay):
			enemy(enemy),
			delay(delay){};
	};

	sf::Clock scriptClock;
	std::queue<ScriptTick*> list;
	bool inited;


public:
	void addEnemy(Enemy* enemy, int delay);

	// ScriptClock
	sf::Clock& getClock();
	void setClock(sf::Clock&);

	// Init
	bool getInit();
	void setInit(bool);

	// Process
	void process(std::list<Shooter*>&);




};