//
// Created by per on 4/19/21.
//

#include <Enemy.h>
#include <effolkronium/random.hpp>
#include <Script.h>
#include <spdlog/spdlog.h>

#include "World.h"

Script::Script(World* world, Constants::ResourceC::Scripts scriptID):
world(world),
enemyTime(sf::milliseconds(0)),
powerupTime(sf::milliseconds(0)),
scriptID(scriptID),
totalEnemies(0)

{
}


void Script::processEnemy(){

    //####################################//
    //######Enemy script processing#######//
    //####################################//
    if(enemies.empty()){
        // Nothing to process
        return;
    }

    // Get enemy in front of queue
    auto& enemy = enemies.front();
    if(enemyTime.asMilliseconds() < enemy->getDelay()){
        // Delay is not yet met. Nothing to do.
        return;
    }

    SPDLOG_INFO("Spawning Enemy#{}", (void*)enemy.get());
    // Push enemy to the objects list (from world)
    world->addEnemyObject(enemy);

    // Pop enemy from script queue.
    enemies.pop();

    // Reset delay timer
    enemyTime = sf::milliseconds(0);

}


void Script::processPowerUp(){
    //####################################//
    //######Enemy script processing#######//
    //####################################//
    if (powerups.empty()) {
        // No power ups
        return;
    }

    // Create a new script tick of the powerup
    auto& pwrUp = powerups.front();

    if(powerupTime.asMilliseconds() < pwrUp->delay) {
        return;
    }

    SPDLOG_INFO("Spawning Powerup#{}", (void*)pwrUp.get());
    // Push enemy to the objects list (from world)
    world->addPowerupObject(pwrUp);


    // Pop snd push from/to queue
    powerups.pop();

    if (pwrUp->repeat == 1)
    {
        powerups.push(pwrUp);
    }

    // Reset the clock
    powerupTime = sf::milliseconds(0);
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
bool Script::process()
{
    enemyTime += world->getTimeStep();
    powerupTime += world->getTimeStep();

    processEnemy();
    processPowerUp();



    // Script is done (no enemies will spawn) IF EMPTY
    // Script is still running if not empty
    return !enemies.empty();
}

void Script::loadEnemies(std::list <ScriptTick> &queue) {

    for(auto& st: queue){

        // Create a new enemy with the information provided by the script tick
        std::shared_ptr<Enemy> e1 = std::make_shared<Enemy>(
                world->getRenderer(),
                st.pathQueue,
                st.emoteQueue,
                st.type,
                st.repeat,
                st.delay,
                world->getBulletFactory(),
                world->getBullets(),
                world->getResourceHandler(),
                world->getShooterObjects(),
                world->getTimeStep());

        enemies.push(e1);
        totalHealth = e1->getStartHealth();
    }
    totalEnemies = enemies.size();
}

void Script::loadPowerUps(std::list<ScriptTick> &queue) {

    for(auto& st: queue){
        // Create a new powerup
        VectorN path = st.pathQueue.front();


        (path.x == -1) ? path.x = (float)effolkronium::random_static::get(0, (int)world->getWindow().getSize().x + 1)  : path.x;
        (path.y == -1) ? path.y = (float)effolkronium::random_static::get(0, (int)world->getWindow().getSize().y + 1) : path.y;

        std::shared_ptr<Powerup> p1 = std::make_shared<Powerup>(
                world->getRenderer(), path, st.type, st.repeat, st.delay, world->getResourceHandler(),
                world->getTimeStep());

        // Push the powerup
        powerups.push(p1);


    }

}

size_t Script::getTotalEnemies() const {
    return totalEnemies;
}

Constants::ResourceC::Scripts Script::getScriptID(){
    return scriptID;
}

int Script::getEnemyTotalHealth() const {
    return totalHealth;
}
