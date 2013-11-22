#include "Script.h"
#include "Log.h"

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
	this->inited = status;
}

// Process
void Script::process()
{
	if(this->getInit())
	{
		// Do processing
		ScriptTick* e = this->list.front();

		this->list.pop();


	}


}
