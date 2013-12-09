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
	countdownSong(resourceHandler->getSound(ResourceHandler::Sound::COUNTDOWN)),
	ingameSong(ingameSong),
	gameOver(false),
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
		addObject(std::shared_ptr<Player>(player));
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
	if (objects.size() > 1 || scriptRunning)
	{
		for (std::list<std::shared_ptr<Shooter>>::iterator i = objects.begin(); i != objects.end();)
		{
			// Process the object (Player and enemy)
			(*i)->process();


			///////////////////////////////////
			////////////CLEANUP////////////////
			///////////////////////////////////
			// Checks if the object's delete flag is set
			if ((*i)->getDeleted())
			{
				// Checks if the object is a enemy
				if ((*i)->getType() == Shooter::ShooterType::ENEMY)
				{
					// Increment the score value of the enemy
					player->addScore((*i)->getScoreValue());
				}
				// Checks if the object is a player
				else if ((*i)->getType() == Shooter::ShooterType::PLAYER)
				{
					// Sets gameOver flag
					gameOver = true;

					// Checks if the playerscore higher than 0 and writes to the Highscore.
					int multiplier = ((getHardMode()) ? 2 : 1); // Hardmode multiplier.
					if (player->getPlayerScore() > 0)
					{
						resourceHandler->writeHighScoreScore(player->getPlayerScore() * multiplier, currentScript); // Write highscore
					}

				}
				// Delete the object from the list and continiue looping
				i = objects.erase(i);
			}
			else
			{
				++i;
			}
		}
	}
	// Nothing more to do, game is over.
	else 
	{
		// Sets gameOver flag;
		gameOver = true;

		// Writes to the Highscore.
		int multiplier = ((getHardMode()) ? 2 : 1); // Hardmode multiplier.
		if (currentScript != -1) resourceHandler->writeHighScoreScore(player->getPlayerScore() * multiplier, currentScript); // Write highscore
	}

	///////////////////////////////////
	// Bullet processing and cleanup //
	///////////////////////////////////
	if (!bullets.empty())
	{
		for (std::list<std::unique_ptr<Bullet>>::iterator it = bullets.begin(); it != bullets.end();)
		{
			// Process the bullets
			(*it)->process();

			///////////////////////////////////
			////////////CLEANUP////////////////
			///////////////////////////////////
			// Checks if the delete flag is set for the bullet
			if ((*it)->getDeleted())
			{
				// Return the bullet to the bullet factory
				bFactory.returnObject(std::move(*it));

				// Delete the pointer from the list and set the iterator
				it = bullets.erase(it);
			}
			else
			{
				// Increment the iterator
				++it;
			}
		}
	}

	///////////////////////////////////
	// Powerup processing and cleanup //
	///////////////////////////////////
	if (!powerups.empty())
	{

		for (std::list<std::shared_ptr<Powerup>>::iterator it = powerups.begin(); it != powerups.end();)
		{
			// Process the power ups
			(*it)->process();

			// Check if player and powerup collide
			bool collision = (*it)->hitDetection(player);

			// Collision happened, the power up was set to deleted and actions will now happen inside this if clause
			if (collision)
			{
				player->powerUp((*it)->getPowerUpType());
			}

			///////////////////////////////////
			////////////CLEANUP////////////////
			///////////////////////////////////
			// Checks if the powerups's delete flag is set
			if ((*it)->getDeleted())
			{
				it = powerups.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

}

/// <summary>
/// Draws the player stats. (Tunneled function because of alternative view)
/// </summary>
void World::drawStats()
{
	player->drawStats(resourceHandler->getHighScores()[(ResourceHandler::Scripts)currentScript]);


}

/// <summary>
/// Determines whether [is game over].
/// </summary>
/// <returns></returns>
bool World::isGameOver()
{
	return gameOver;
}

/// <summary>
/// Adds a object to the scene
/// </summary>
/// <param name="object">The object thats requested for addition</param>
void World::addObject(std::shared_ptr<Shooter> object)
{
	//LOGD("Object#" << object << " | Object Size: " << this->objects.size());
	this->objects.push_back(object);
}

/// <summary>
/// Adds a bullet to the bullet list
/// </summary>
/// <param name="bullet">The bullet.</param>
void World::addBullet(std::unique_ptr<Bullet> bullet)
{
	//LOGD("Object#" << object << " | Object Size: " << this->objects.size());
	this->bullets.push_back(std::move(bullet));
}



/// <summary>
/// Draws all scene objects
/// </summary>
void World::draw()
{
	// Draw background
	bg.draw();

	// Bullets 
	for (auto &it : bullets)
	{
		it->draw();
	}

	// Players and enemies
	for (auto &it : objects)
	{
		it->draw();
	}

	// Power Ups
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
/// Gets hard mode flag
/// </summary>
/// <returns></returns>
const bool World::getHardMode()
{
	return hardMode;
}