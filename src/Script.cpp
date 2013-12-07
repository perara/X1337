#include "Script.h"
#include "Log.h"
#include "Object.h"
#include "Enemy.h"
#include "BulletFactory.h"
#include "Bullet.h"


void Script::addEnemy(int delay, std::queue<sf::Vector3f> pathQueue, int type, int repeat)
{
	LOGD("Adding new enemy template to pool");
	ScriptTick tick(delay, pathQueue, type, repeat);


	enemyList.push(tick);

}

// ScriptClock
sf::Clock& Script::getClock()
{
	return scriptClock;
}

void Script::setClock(sf::Clock& clock)
{
	scriptClock = clock;
}

// Init
bool Script::getInit()
{
	return this->inited;

}
void Script::setInit(bool status)
{
	if (status) getClock().restart();
	inited = status;
}

std::string Script::getScriptTitle()
{
	return scriptTitle;
}

void Script::setScriptTitle(std::string scriptTitle)
{
	this->scriptTitle = scriptTitle;
}


void Script::setScriptEnumVal(int enumVal)
{
	scriptEnumVal = enumVal;
}

int Script::getScriptEnumVal()
{
	return scriptEnumVal;
}

// Process
bool Script::process(sf::RenderWindow& window,
	std::list<std::shared_ptr<Shooter>>& objects,
	std::list<std::shared_ptr<Powerup>>& powerups,
	std::list<std::unique_ptr<Bullet>>& bullets,
	BulletFactory& bFactory,
	std::unique_ptr<ResourceHandler>& resourceHandler,
	const sf::Time& timeStep)
{
	// Do processing
	if (!enemyList.empty())
	{

		ScriptTick e = enemyList.front();
		//std::cout <<this->getInit() << " and "  << this->getClock().getElapsedTime().asMilliseconds() << " and " << e->delay << std::endl;
		if (
			this->getInit() &&
			this->getClock().getElapsedTime().asMilliseconds() > e.delay
			)
		{

			if (e.type > 0)
			{
				std::shared_ptr<Enemy> e1 = std::shared_ptr<Enemy>(new Enemy(
					window,
					e.pathQueue,
					e.type,
					e.repeat,
					bFactory,
					bullets,
					resourceHandler,
					timeStep));
				LOGD("Spawning Enemy#" << e1);
				objects.push_back(e1);
			}
			else // This means the type is less that 0, 0 and below is powerUps
			{
				std::shared_ptr<Powerup> p1 = std::shared_ptr<Powerup>(new Powerup(window, e.pathQueue.front(), e.type, resourceHandler, timeStep));
				powerups.push_back(p1);
			}


			enemyList.pop();


			this->getClock().restart();
		}
	}
	else
	{
		return false; // Script is done
	}
	return true; // Script is still running
}
