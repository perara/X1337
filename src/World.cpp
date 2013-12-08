#include "World.h"
#include "ResourceHandler.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Background.h"
#include "Log.h"
#include "GameShape.h"



World::World(sf::RenderWindow& window,
	std::unique_ptr<ResourceHandler>& resourceHandler,
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
		ingameSong.play();
	}
	else
	{
		script = resourceHandler->getScriptById(scriptNum);
		// Initialize Background
		bg.addBackground(resourceHandler->getTexture(ResourceHandler::Texture::BACKGROUND3), true);
		currentScript = scriptNum;
		addObject(std::shared_ptr<Player>(player));
		countdownSong.play();
	}
	startSound();

}

World::~World()
{
	LOGD("World deconstructor called");
}

void World::startSound()
{
	ingameSong.play();
}

void World::stopSound()
{
	if (countdownSong.getStatus() != 0) countdownSong.stop();
	if (ingameSong.getStatus() != 0) ingameSong.stop();
}

void World::process()
{
	bg.process();

	// Process the loaded script IF countdown is done and its not a demo (Demo dont have countdown)
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
			// Process
			(*i)->process();

			// Cleanup
			if ((*i)->getDeleted())
			{ // If the bullet is up for deletion
				if ((*i)->getType() == Shooter::ShooterType::ENEMY)
				{
					player->addScore((*i)->getScoreValue());
				}
				else if ((*i)->getType() == Shooter::ShooterType::PLAYER)
				{
					gameOver = true;
					int multiplier = ((getHardMode()) ? 2 : 1); // Hardmode multiplier.
					if (player->getPlayerScore() > 0)
					{
						resourceHandler->writeHighScoreScore(player->getPlayerScore() * multiplier, currentScript); // Write highscore
					}
				}

				i = objects.erase(i);
			}
			else
			{
				++i;
			}
		}
	}
	else // Nothing more to do, game is over.
	{
		gameOver = true;
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
			// Process
			(*it)->process();

			// Cleanup
			if ((*it)->getDeleted())
			{
				bFactory.returnObject(std::move(*it));
				it = bullets.erase(it);
			}
			else
			{
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
			// Process
			(*it)->process();

			// Check if player and powerup collide
			bool collision = (*it)->hitDetection(player);

			// Collision happened, the power up was set to deleted and actions will now happen inside this if clause
			if (collision)
			{
				player->powerUp((*it)->getPowerUpType());
			}

			// Cleanup
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

void World::drawStats()
{
	player->drawStats(resourceHandler->getHighScores()[(ResourceHandler::Scripts)currentScript]);


}

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

void World::input(sf::Event& event)
{
	this->player->input(event);
}

const bool World::getHardMode()
{
	return hardMode;
}