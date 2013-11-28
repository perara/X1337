#include "Script.h"
#include "Log.h"
#include "Object.h"
#include "Enemy.h"



void Script::addEnemy(Enemy* enemy, int delay)
{
	LOGD("Adding new enemy to queue pool (Enemy#" << enemy << ")");
	ScriptTick* tick = new ScriptTick(enemy, delay);


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

std::string Script::getScriptTitle()
{
	return this->scriptTitle;
}

void Script::setScriptTitle(std::string scriptTitle)
{
	this->scriptTitle = scriptTitle;
}


void Script::setScriptEnumVal(int enumVal)
{
	this->scriptEnumVal = enumVal;
}

int Script::getScriptEnumVal()
{
	return this->scriptEnumVal;
}

// Process
void Script::process(std::list<Shooter*>& objects)
{
	// Do processing
	if(!this->list.empty())
	{

		ScriptTick* e = this->list.front();
		//std::cout <<this->getInit() << " and "  << this->getClock().getElapsedTime().asMilliseconds() << " and " << e->delay << std::endl;
		if(
			this->getInit() &&
			this->getClock().getElapsedTime().asMilliseconds() > e->delay
			)

		{
			LOGD("Spawning Enemy#" << e->enemy);
			objects.push_back(e->enemy);


			this->list.pop();


			this->getClock().restart();
		}
	}

}
