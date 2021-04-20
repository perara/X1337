#include "../include/World.h"
#include <memory>
#include <spdlog/spdlog.h>
#include <Renderer.h>
#include "../include/Enemy.h"
#include "../include/Log.h"
#include "../include/GameShape.h"



/// <summary>
/// Initializes a new instance of the <see cref="World"/> class.
/// </summary>
/// <param name="window">The render window</param>
/// <param name="resourceHandler">The resource handler.</param>
/// <param name="timeStep">The time step which is set constant in the game engine</param>
/// <param name="demo">The demo flag, which is determined by the state world is initiated from</param>
/// <param name="scriptNum">Which script should be runned in this world instance</param>
/// <param name="hardMode">Weither its hardmode or not.</param>
/// <param name="ingameSong">Selected ingame sound</param>
World::World(Renderer& window,
             std::shared_ptr<ResourceManager>& resourceHandler,
             const sf::Time& timeStep,
             const Constants::ResourceC::Scripts scriptNum,
             const bool hardMode,
             sf::Sound& ingameSong)
	: Scene(window, resourceHandler)
	, bg(Background(window))
	, bFactory(BulletFactory(window, 1000, timeStep, resourceHandler))
	, player(std::make_shared<Player>(window, sf::Vector2f(100, 250), 10, bFactory, bullets, resourceHandler, timeStep, hardMode, objects))
    , ingameSong(ingameSong)
    , countdownSong(resourceHandler->getSound(Constants::ResourceC::Sound::MUSIC_COUNTDOWN))
    , timeStep(timeStep)
    , hardMode(hardMode)
    , gameOver(0)
    , stageProgress(0)
{

		script = resourceHandler->getScript(scriptNum)->load(this);

		if(this->isGameMenu()){
            ingameSong.play();
            bg.addBackground(resourceHandler->getTexture(Constants::ResourceC::Texture::BACKGROUND2), false);
		}else{
            // Aditionally adds the player and starts the countdown soundtrack
            bg.addBackground(resourceHandler->getTexture(Constants::ResourceC::Texture::BACKGROUND3), true);
            this->objects.push_back(std::shared_ptr<Player>(player));
            countdownSong.play();
		}

	// Runs the sounds (Had to be done frome seperate function for some reason, (dies in the constructor?)
	startSound();
}

bool World::isGameMenu(){
    return script->getScriptID() == Constants::ResourceC::Scripts::GAME_MENU;
}

bool World::hasGameScript(){
    return script && !isGameMenu();
}

/// <summary>
/// Finalizes an instance of the <see cref="World"/> class.
/// </summary>
World::~World()
{
    SPDLOG_INFO("World deconstructor called");
}

/// <summary>
/// Starts the sound.
/// </summary>
void World::startSound()
{
	ingameSong.play();
}

/// <summary>
/// Stops the sound.
/// </summary>
void World::stopSound()
{
	// Check weither the songs are playing or not. If a song is playing, we stop them.
	if (countdownSong.getStatus() != 0) countdownSong.stop();
	if (ingameSong.getStatus() != 0) ingameSong.stop();
}

/// <summary>
/// Processes this instance.
/// </summary>
void World::process()
{
	// Process the background
	bg.process();

	// Process the loaded script IF countdown is done and its not a demo (Demo does not have countdown)

	if (countdownSong.getStatus() == 0 || isGameMenu()){
        script->process();
	}



	///////////////////////////////////
	// Object processing and cleanup //
	///////////////////////////////////
	if (!objects.empty())
	{
		for (auto objectIt = objects.begin(); objectIt != objects.end();)
		{
			// Process the object (Player and enemy)
			(*objectIt)->process();

			///////////////////////////////////
			////////////CLEANUP////////////////
			///////////////////////////////////
			// Checks if the object's delete flag is set
			if ((*objectIt)->getDeleted())
			{
				// Checks if the object is a enemy
				if ((*objectIt)->getType() == Constants::ShooterType::ENEMY)
				{
					// Increment the score value of the enemy
					player->addScore((*objectIt)->getScoreValue());
				}

				// Delete the object from the list and continiue looping
				objectIt = objects.erase(objectIt);
			}
			else
			{
				++objectIt;

			} // -- if end
		} // -- for end
	} // -- if end

	///////////////////////////////////
	// Bullet processing and cleanup //
	///////////////////////////////////
	if (!bullets.empty())
	{
		for (auto bulletIt = bullets.begin(); bulletIt != bullets.end();)
		{
			// Process the bullets
			(*bulletIt)->process();

			///////////////////////////////////
			////////////CLEANUP////////////////
			///////////////////////////////////
			// Checks if the delete flag is set for the bullet
			if ((*bulletIt)->getDeleted())
			{
				// Return the bullet to the bullet factory
				bFactory.returnObject(*bulletIt);

				// Delete the pointer from the list and set the iterator
				bulletIt = bullets.erase(bulletIt);
			}
			else
			{
				++bulletIt;
			} // -- if end
		} // -- for end
	} // -- if end

	///////////////////////////////////
	// Powerup processing and cleanup //
	///////////////////////////////////
	if (!powerups.empty())
	{

		for (auto powerupIt = powerups.begin(); powerupIt != powerups.end();)
		{
			// Process the power ups
			(*powerupIt)->process();

			// Check if player and powerup collide
			bool collision = (*powerupIt)->hitDetection(player);

			// Collision happened, the power up was set to deleted and actions will now happen inside this if clause
			if (collision)
			{
				player->powerUp((*powerupIt)->getPowerUpType());
			}

			///////////////////////////////////
			////////////CLEANUP////////////////
			///////////////////////////////////
			// Checks if the powerups's delete flag is set
			if ((*powerupIt)->getDeleted())
			{
				powerupIt = powerups.erase(powerupIt);
			}
			else
			{
				++powerupIt;
			} // -- if end
		} // -- for end
	} // -- if end

	// Update the stage progression
	stageProgress = ((float)player->getTotalDamageDone() / (float)script->getEnemyTotalHealth()) * 100.0f; // Stage progress in percent

	// Check for gameOver
	evaluateGameOver();
}

/// <summary>
/// Draws the player stats. (Tunneled function because of alternative view)
/// </summary>
void World::drawStats()
{
	player->drawStats(resourceHandler->getHighScores()[(Constants::ResourceC::Scripts)script->getScriptID()]);
}

/// <summary>
/// Determines whether [is game over]. 0= not game over, 1 = game over, 2 = game over and winning
/// </summary>
/// <returns></returns>
int World::isGameOver() const
{
	return gameOver;
}

/// <summary>
/// Draws all scene objects
/// </summary>
void World::draw()
{
	// Draw background
	bg.draw();

	// Draw Stage Progress
	drawGameProgress();

	// Draw Bullets 
	for (auto &it : bullets)
	{
		it->draw();
	}

	// Draw Players and enemies
	for (auto &it : objects)
	{
		it->draw();
	}

	// Draw Power Ups
	for (auto& it : powerups)
	{
		it->draw();
	}

	// Draw "Prepare Text"
	if (countdownSong.getStatus() != 0 && !isGameMenu())
	{
		sf::Text txtPrep;
		txtPrep.setFont(resourceHandler->getFont(Constants::ResourceC::Fonts::SANSATION));
		txtPrep.setCharacterSize(50);
		txtPrep.setString("Prepare for game!");
		txtPrep.setFillColor(sf::Color::White);
		txtPrep.setPosition((
                                    renderer.getView().getSize().x / 2) - (txtPrep.getGlobalBounds().width / 2),
                            (renderer.getView().getSize().y / 2) - (txtPrep.getGlobalBounds().height));
		renderer.draw(txtPrep);
	}
}

/// <summary>
/// Inputs handler for world, tunelled to the appropriate places)
/// </summary>
/// <param name="event">The event.</param>
void World::input(sf::Event& event)
{
	this->player->input(event);
}


/// <summary>
/// Draws the game progress text
/// </summary>
void World::drawGameProgress()
{
	if (!isGameMenu())
	{
		sf::Text txtProgress;
		txtProgress.setFont(resourceHandler->getFont(Constants::ResourceC::Fonts::SANSATION));
		txtProgress.setCharacterSize(12);
		txtProgress.setString("Stage Progress: " + std::to_string(stageProgress) + "%");
		txtProgress.setPosition(10, renderer.getView().getSize().y - txtProgress.getGlobalBounds().height * 2);
		renderer.draw(txtProgress);
	}
}

/// <summary>
/// Evaluates the gameOver state
/// </summary>
void World::evaluateGameOver()
{
	// If the stage is complete
	if (stageProgress == 100)
	{
		gameOver = 2;
	}
	else if (player->getDeleted())
	{
		gameOver = 1;
	}

	// Checks if:
	// 1. Script is set correctly
	// 2. Playerscore is above 0
	// 3. Game over is not false (0)
	if (hasGameScript() &&
		player->getPlayerScore() > 0 &&
		gameOver != 0
		)

	{
		// Writes to the Highscore.
		float multiplier = (hardMode ? 2.0f : 1.0f); // Hardmode multiplier.
		resourceHandler->writeHighScoreScore(player->getPlayerScore() * multiplier, script->getScriptID()); // Write highscore
	}
}

const sf::Time &World::getTimeStep() {
    return timeStep;
}

BulletFactory &World::getBulletFactory() {
    return bFactory;
}

std::list<std::unique_ptr<Bullet>> &World::getBullets() {
    return bullets;
}

std::shared_ptr<ResourceManager> &World::getResourceHandler() {
    return resourceHandler;
}

sf::RenderWindow &World::getWindow() {
    return renderer.getWindow();
}

Renderer& World::getRenderer(){
    return renderer;
}

void World::addEnemyObject(std::shared_ptr<Enemy> &enemy) {
    objects.push_back(enemy);
}

std::list<std::shared_ptr<Shooter>>& World::getShooterObjects() {
    return objects;
}

void World::addPowerupObject(std::shared_ptr<Powerup> &powerup) {
    powerups.push_back(powerup);
}

std::list<std::shared_ptr<Powerup>>& World::getPowerupObjects() {
    return powerups;
}
