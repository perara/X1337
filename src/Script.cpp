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
/// <param name="emoteQueue">A queue which contains emoteNames. This is a sorted list</param>
/// <param name="type">The type.</param>
/// <param name="repeat">The repeat.</param>
void Script::addEnemy(int delay, std::queue<VectorN> pathQueue, std::list<std::pair<int, std::string>> emoteQueue, int type, int repeat)
{
	LOGD("Adding new enemy template to pool");
	ScriptTick tick(delay, pathQueue, emoteQueue, type, repeat);
	enemyList.push(tick);


}

/// <summary>
/// Adds a power up item to the powerup queue..
/// </summary>
/// <param name="delay">The delay.</param>
/// <param name="spawnPoint">The spawn point.</param>
/// <param name="type">The type.</param>
/// <param name="repeat">The repeat.</param>
void Script::addPowerUp(int delay, VectorN spawnPoint, int type, int repeat)
{
	std::queue<VectorN> queue;
	queue.push(spawnPoint);

	std::list<std::pair<int, std::string>> emoteQueue;

	LOGD("Adding new powerUp template to pool");
	ScriptTick tick(delay, queue, emoteQueue, type, repeat);
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
		startEnemyListSize = enemyList.size();
		enemyTime = sf::milliseconds(0);
		powerupTime = sf::milliseconds(0);
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
	enemyTime += timeStep;
	powerupTime += timeStep;

	//####################################//
	//######Enemy script processing#######//
	//####################################//
	if (!enemyList.empty())
	{
		// Get enemy in front of queue
		ScriptTick e = enemyList.front();
		if (this->getInit() &&
			enemyTime.asMilliseconds() > e.delay)
		{

			// Create a new enemy with the information provided by the script tick
			std::shared_ptr<Enemy> e1 = std::shared_ptr<Enemy>(new Enemy(
				window,
				e.pathQueue,
				e.emoteQueue,
				e.type,
				e.repeat,
				bFactory,
				bullets,
				resourceHandler,
				timeStep));
			LOGD("Spawning Enemy#" << e1);

			// Push enemy to the objects list (from world)
			objects.push_back(e1);

			// Pop the garbage scripttick
			enemyList.pop();

			enemyTime = sf::milliseconds(0);
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
			powerupTime.asMilliseconds() > pwrUp.delay)
		{

			// Create a new powerup
			VectorN path = pwrUp.pathQueue.front();

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
			powerupTime = sf::milliseconds(0);
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

/// <summary>
/// Sets the audio description string
/// </summary>
/// <param name="audioDesc">The audio description string name (Mapped in ResourceHandler)</param>
void Script::setAudioDesc(std::string audioDesc)
{
	this->audioDesc = audioDesc;
}

/// <summary>
/// Gets the audio description string
/// </summary>
/// <returns>The audio description string name (Mapped in ResourceHandler)</returns>
std::string Script::getAudioDesc()
{
	return this->audioDesc;
}

/// <summary>
/// Sets the lore string
/// </summary>
/// <param name="lore">Lore string</param>
void Script::setLore(std::string lore)
{
	this->lore = lore;
}

/// <summary>
/// Returns the lore string
/// </summary>
/// <returns>String with lore</returns>
std::string Script::getLore()
{
	return this->lore;
}

/// <summary>
/// Gets the start size of the enemy list.
/// </summary>
/// <returns>integer with size</returns>
const int Script::getStartEnemyListSize()
{
	return startEnemyListSize;
}

/// <summary>
/// Gets the size of the enemy list.
/// </summary>
/// <returns>size of enemy list</returns>
const int Script::getEnemyListSize()
{
	return enemyList.size();
}

void Script::setPortraitString(std::string portraitString)
{
	this->portraitString = portraitString;
}

std::string Script::getPortraitString()
{
	return portraitString;
}