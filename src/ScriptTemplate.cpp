#include "../include/ScriptTemplate.h"

#include <memory>
#include <utility>
#include <spdlog/spdlog.h>
#include "../include/Enemy.h"
#include <World.h>


std::unique_ptr<Script> ScriptTemplate::load(World* world){
    auto s = std::make_unique<Script>(world, (Constants::ResourceC::Scripts)getScriptEnumVal());

    s->loadEnemies(templateEnemies);
    s->loadPowerUps(templatePowerups);

    return s;
}


/// <summary>
/// Adds a enemy to the script queue
/// </summary>
/// <param name="delay">The delay.</param>
/// <param name="pathQueue">The path queue.</param>
/// <param name="emoteQueue">A queue which contains emoteNames. This is a sorted list</param>
/// <param name="type">The type.</param>
/// <param name="repeat">The repeat.</param>
void ScriptTemplate::addEnemy(
        int delay,
        std::queue<VectorN> pathQueue,
        std::list<std::pair<int, std::string>> emoteQueue,
        int type,
        int repeat
        )
{
    SPDLOG_INFO("Adding new enemy template to pool");

	ScriptTick tick(delay,std::move(pathQueue),std::move(emoteQueue), type, repeat);
	templateEnemies.push_back(tick);


}

/// <summary>
/// Adds a power up item to the powerup queue..
/// </summary>
/// <param name="delay">The delay.</param>
/// <param name="spawnPoint">The spawn point.</param>
/// <param name="type">The type.</param>
/// <param name="repeat">The repeat.</param>
void ScriptTemplate::addPowerUp(int delay, const VectorN& spawnPoint, int type, int repeat)
{
	std::queue<VectorN> queue;
	queue.push(spawnPoint);

	std::list<std::pair<int, std::string>> emoteQueue;

    SPDLOG_INFO("Adding new powerUp template to pool");
	ScriptTick tick(delay, queue, emoteQueue, type, repeat);
	templatePowerups.emplace_back(tick);

}

/// <summary>
/// Returns the script title.
/// </summary>
/// <returns>Script status</returns>
std::string ScriptTemplate::getScriptTitle()
{
	return scriptTitle;
}

/// <summary>
/// Sets the script title.
/// </summary>
/// <param name="scriptTitle">The script title.</param>
void ScriptTemplate::setScriptTitle(std::string _scriptTitle)
{
	this->scriptTitle = std::move(_scriptTitle);
}


/// <summary>
/// Sets the script enum value.
/// </summary>
/// <param name="enumVal">The enum value.</param>
void ScriptTemplate::setScriptEnumVal(int enumVal)
{
	scriptEnumVal = enumVal;
}

/// <summary>
/// Gets the script enum value.
/// </summary>
/// <returns>The script enumeration value (defined in the xml) </returns>
int ScriptTemplate::getScriptEnumVal() const
{
	return scriptEnumVal;
}


/// <summary>
/// Sets the audio description string
/// </summary>
/// <param name="audioDesc">The audio description string name (Mapped in ResourceHandler)</param>
void ScriptTemplate::setAudioDesc(std::string _audioDesc)
{
	this->audioDesc = std::move(_audioDesc);
}

/// <summary>
/// Gets the audio description string
/// </summary>
/// <returns>The audio description string name (Mapped in ResourceHandler)</returns>
std::string ScriptTemplate::getAudioDesc()
{
	return this->audioDesc;
}

/// <summary>
/// Sets the lore string
/// </summary>
/// <param name="lore">Lore string</param>
void ScriptTemplate::setLore(std::string _lore)
{
	this->lore = std::move(_lore);
}

/// <summary>
/// Returns the lore string
/// </summary>
/// <returns>String with lore</returns>
std::string ScriptTemplate::getLore()
{
	return this->lore;
}


void ScriptTemplate::setPortraitString(std::string _portraitString)
{
	this->portraitString = std::move(_portraitString);
}

std::string ScriptTemplate::getPortraitString()
{
	return portraitString;
}