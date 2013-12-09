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

void Script::addPowerUp(int delay, sf::Vector3f spawnPoint, int type, int repeat)
{
	std::queue<sf::Vector3f> queue;
	queue.push(spawnPoint);

	LOGD("Adding new powerUp template to pool");
	ScriptTick tick(delay, queue, type, repeat);
	powerupList.push(tick);

}

// Init
bool Script::getInit()
{
	return this->inited;

}
void Script::setInit(bool status)
{
	if (status)
	{
		enemyClock.restart();
		powerupClock.restart();
	}

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
	std::shared_ptr<ResourceHandler>& resourceHandler,
	const sf::Time& timeStep)
{
	//####################################//
	//######Enemy script processing#######//
	//####################################//

	if (!enemyList.empty())
	{
		// Get enemy in front of queue
		ScriptTick e = enemyList.front();
		if (this->getInit() &&
			enemyClock.getElapsedTime().asMilliseconds() > e.delay)
		{
			LOGD("Spawning Enemy#" << e1);

			// Create a new enemy with the information provided by the script tick
			std::shared_ptr<Enemy> e1 = std::shared_ptr<Enemy>(new Enemy(
				window,
				e.pathQueue,
				e.type,
				e.repeat,
				bFactory,
				bullets,
				resourceHandler,
				timeStep));

			// Push enemy to the objects list (from world)
			objects.push_back(e1);

			// Pop the garbage scripttick
			enemyList.pop();

			// Reset the clock
			enemyClock.restart();
		}
	}


	//####################################//
	//######Enemy script processing#######//
	//####################################//
	if (!powerupList.empty())
	{

		// Create a new script tick of the powerup
		ScriptTick pwrUp = powerupList.front();
		// Check weither the clock has move enough
		if (this->getInit() &&
			powerupClock.getElapsedTime().asMilliseconds() > pwrUp.delay)
		{

			// Create a new powerup

			sf::Vector3f path = pwrUp.pathQueue.front();

			(path.x == -1) ? path.x = rand() % window.getSize().x + 1 : path.x;
			(path.y == -1) ? path.y = rand() % window.getSize().y + 1 : path.y;

			std::shared_ptr<Powerup> p1 = std::shared_ptr<Powerup>(
				new Powerup(window, path, pwrUp.type, resourceHandler, timeStep));

			// Push the powerup 
			powerups.push_back(p1);

			// Pop snd push from/to queue
			powerupList.pop();

			if (pwrUp.repeat == 1)
			{
				powerupList.push(pwrUp);
			}

			// Reset the clock
			powerupClock.restart();
		}

	}


	if (enemyList.empty())
	{
		return false; // Script is done (no enemies will spawn)
	}
	else
	{
		return true; // Script is still running
	}
}
