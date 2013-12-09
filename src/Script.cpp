#include "Script.h"
#include "Log.h"
#include "Object.h"
#include "Enemy.h"
#include "BulletFactory.h"
#include "Bullet.h"


/// <summary>
/// Adds a enemy to the script queue
/// </summary>
/// <param name="delay">The delay.</param>
/// <param name="pathQueue">The path queue.</param>
/// <param name="type">The type.</param>
/// <param name="repeat">The repeat.</param>
void Script::addEnemy(int delay, std::queue<sf::Vector3f> pathQueue, int type, int repeat)
{
	LOGD("Adding new enemy template to pool");
	ScriptTick tick(delay, pathQueue, type, repeat);
	enemyList.push(tick);


}

/// <summary>
/// Adds a power up item to the powerup queue..
/// </summary>
/// <param name="delay">The delay.</param>
/// <param name="spawnPoint">The spawn point.</param>
/// <param name="type">The type.</param>
/// <param name="repeat">The repeat.</param>
void Script::addPowerUp(int delay, sf::Vector3f spawnPoint, int type, int repeat)
{
	std::queue<sf::Vector3f> queue;
	queue.push(spawnPoint);

	LOGD("Adding new powerUp template to pool");
	ScriptTick tick(delay, queue, type, repeat);
	powerupList.push(tick);

}


/// <summary>
/// Retrieve the init status
/// </summary>
/// <returns>init flag</returns>
bool Script::getInit()
{
	return this->inited;

}
/// <summary>
/// Sets the initialization status
/// </summary>
/// <param name="status">Status flag</param>
void Script::setInit(bool status)
{
	if (status)
	{
		enemyClock.restart();
		powerupClock.restart();
	}

	inited = status;
}

/// <summary>
/// Returns the script title.
/// </summary>
/// <returns>Script status</returns>
std::string Script::getScriptTitle()
{
	return scriptTitle;
}

/// <summary>
/// Sets the script title.
/// </summary>
/// <param name="scriptTitle">The script title.</param>
void Script::setScriptTitle(std::string scriptTitle)
{
	this->scriptTitle = scriptTitle;
}


/// <summary>
/// Sets the script enum value.
/// </summary>
/// <param name="enumVal">The enum value.</param>
void Script::setScriptEnumVal(int enumVal)
{
	scriptEnumVal = enumVal;
}

/// <summary>
/// Gets the script enum value.
/// </summary>
/// <returns>The script enumeration value (defined in the xml) </returns>
int Script::getScriptEnumVal()
{
	return scriptEnumVal;
}

// Process
/// <summary>
/// Processed the script. It takes in all neccsary items in order to process it correctly
/// </summary>
/// <param name="window">The window.</param>
/// <param name="objects">The objects.</param>
/// <param name="powerups">The powerups.</param>
/// <param name="bullets">The bullets.</param>
/// <param name="bFactory">The b factory.</param>
/// <param name="resourceHandler">The resource handler.</param>
/// <param name="timeStep">The time step.</param>
/// <returns></returns>
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
