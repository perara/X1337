#pragma once
#include <queue>
#include <list>
#include <SFML\System\Clock.hpp>

class Enemy;
class Object;


class Script
{
	struct ScriptTick {
		Enemy* enemy;
		int delay;
	} ;

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
	void process(std::vector<Object*>&);




};