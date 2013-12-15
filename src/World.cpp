#include "World.h"
#include "ResourceHandler.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Background.h"
#include "Log.h"
#include "GameShape.h"



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
World::World(sf::RenderWindow& window,
			 std::shared_ptr<ResourceHandler>& resourceHandler,
			 const sf::Time& timeStep,
			 const bool demo,
			 const int scriptNum,
			 const bool hardMode,
			 sf::Sound& ingameSong)
			 :
Scene(window, resourceHandler),
	bg(Background(window)),
	bFactory(BulletFactory(window, 1000, bullets, timeStep, resourceHandler)),
	timeStep(timeStep),
	countdownSong(resourceHandler->getSound(ResourceHandler::Sound::MUSIC_COUNTDOWN)),
	ingameSong(ingameSong),
	gameOver(0),
	stageProgress(0),
	hardMode(hardMode),
	demo(demo),
	player(std::shared_ptr<Player>(new Player(window, sf::Vector2f(100, 250), 10, bFactory, bullets, resourceHandler, timeStep, hardMode)))
{
	if (demo)
	{
		bg.addBackground(resourceHandler->getTexture(ResourceHandler::Texture::BACKGROUND2), false);
		script = resourceHandler->getScript(ResourceHandler::Scripts::GAME_MENU);
		currentScript = scriptNum;
		ingameSong.play();
	}
	else
	{
		bg.addBackground(resourceHandler->getTexture(ResourceHandler::Texture::BACKGROUND3), true);
		script = resourceHandler->getScriptById(scriptNum);
		currentScript = scriptNum;

		// Aditionally adds the player and starts the countdown soundtrack
		this->objects.push_back(std::shared_ptr<Player>(player));
		countdownSong.play();
	}

	// Runs the sounds (Had to be done frome seperate function for some reason, (dies in the constructor?)
	startSound();
}

/// <summary>
/// Finalizes an instance of the <see cref="World"/> class.
/// </summary>
World::~World()
{
	LOGD("World deconstructor called");
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
	bool scriptRunning;
	(countdownSong.getStatus() != 0 && !demo) ?
		scriptRunning = true :
	scriptRunning = script.process(window, objects, powerups, bullets, bFactory, resourceHandler, timeStep);

	///////////////////////////////////
	// Object processing and cleanup //
	///////////////////////////////////
	if (!objects.empty())
	{
		for (std::list<std::shared_ptr<Shooter>>::iterator objectIt = objects.begin(); objectIt != objects.end();)
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
				if ((*objectIt)->getType() == Shooter::ShooterType::ENEMY)
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
		for (std::list<std::unique_ptr<Bullet>>::iterator bulletIt = bullets.begin(); bulletIt != bullets.end();)
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
				bFactory.returnObject(std::move(*bulletIt));

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

		for (std::list<std::shared_ptr<Powerup>>::iterator powerupIt = powerups.begin(); powerupIt != powerups.end();)
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
	stageProgress = (int)(player->getPlayerKills() * ((float)100 / script.getStartEnemyListSize())); // Stage progress in percent

	// Check for gameOver
	evaluateGameOver();
}

/// <summary>
/// Draws the player stats. (Tunneled function because of alternative view)
/// </summary>
void World::drawStats()
{
	player->drawStats(resourceHandler->getHighScores()[(ResourceHandler::Scripts)currentScript]);
}

/// <summary>
/// Determines whether [is game over]. 0= not game over, 1 = game over, 2 = game over and winning
/// </summary>
/// <returns></returns>
int World::isGameOver()
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
	if (countdownSong.getStatus() != 0 && !demo)
	{
		sf::Text txtPrep;
		txtPrep.setFont(resourceHandler->getFont(ResourceHandler::Fonts::SANSATION));
		txtPrep.setCharacterSize(50);
		txtPrep.setString("Prepare for game!");
		txtPrep.setColor(sf::Color::White);
		txtPrep.setPosition((
			window.getView().getSize().x / 2) - (txtPrep.getGlobalBounds().width / 2),
			(window.getView().getSize().y / 2) - (txtPrep.getGlobalBounds().height));
		window.draw(txtPrep);
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
	if(!demo)
	{ 
		sf::Text txtProgress;
		txtProgress.setFont(resourceHandler->getFont(ResourceHandler::Fonts::SANSATION));
		txtProgress.setCharacterSize(12);
		txtProgress.setString("Stage Progress: " + std::to_string(stageProgress) + "%");
		txtProgress.setPosition(10,window.getView().getSize().y - txtProgress.getGlobalBounds().height * 2);
		window.draw(txtProgress);
	}
}

/// <summary>
/// Evaluates the gameOver state
/// </summary>
void World::evaluateGameOver()
{
	// If the stage is complete
	if(stageProgress == 100)
	{
		gameOver = 2;
	}
	else if(player->getDeleted())
	{
		gameOver = 1;
	}

	// Checks if:
	// 1. Script is set correctly
	// 2. Playerscore is above 0
	// 3. Game over is not false (0)
	if (currentScript != -1 && 
		player->getPlayerScore() > 0 &&
		gameOver != 0
		) 
	{
		// Writes to the Highscore.
		int multiplier = (hardMode ? 2 : 1); // Hardmode multiplier.
		resourceHandler->writeHighScoreScore(player->getPlayerScore() * multiplier, currentScript); // Write highscore
	}
}