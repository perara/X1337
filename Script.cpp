#include "Script.h"
#include "Log.h"
#include "Object.h"
#include "Enemy.h"

void Script::addEnemy(Enemy* enemy, int delay)
{
	LOGD("Adding new enemy to queue pool (Enemy#" << enemy << ")");
	ScriptTick* tick = new ScriptTick();
	tick->delay = delay;
	tick->enemy = enemy;


	this->list.push(tick);

}

// ScriptClock
sf::Clock& Script::getClock()
{
	return this->scriptClock;
}

void Script::setClock(sf::Clock& clock)
{
	this->scriptClock = clock;
}

// Init
bool Script::getInit()
{
	return this->inited;

}
void Script::setInit(bool status)
{
	if(status) this->getClock().restart();
	this->inited = status;
}

// Process
void Script::process(std::list<Object*>& objects)
{
	// Do processing
	if(!this->list.empty())
	{
		ScriptTick* e = this->list.front();
		if(
			this->getInit() &&
			this->getClock().getElapsedTime().asMilliseconds() > e->delay
			)

		{
			LOGD("Spawning Enemy#" << e->enemy);
			objects.push_back(e->enemy);

			if(this->list.size() > 0)
				this->list.pop();

			this->getClock().restart();
		}
	}


}
