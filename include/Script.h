//
// Created by per on 4/19/21.
//

#ifndef X1337_SCRIPT_H
#define X1337_SCRIPT_H

#include "ScriptTick.h"

class World;
class Enemy;

class Script {
private:
    World* world;

    sf::Time enemyTime;
    sf::Time powerupTime;

    std::queue<std::shared_ptr<Powerup>> powerups;
    std::queue<std::shared_ptr<Enemy>> enemies;
    const Constants::ResourceC::Scripts scriptID;

    unsigned long totalEnemies;

public:
    void loadEnemies(std::list<ScriptTick> &queue);

    void loadPowerUps(std::list<ScriptTick> &queue);

    Script(World* world, Constants::ResourceC::Scripts scriptID);

    // Process
    bool process();

    size_t getTotalEnemies() const;
    Constants::ResourceC::Scripts getScriptID();

    int getEnemyTotalHealth() const;

    void processEnemy();

    void processPowerUp();

    int totalHealth;
};


#endif //X1337_SCRIPT_H
