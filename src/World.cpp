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
			 const bool hardMode):
Scene(window, resourceHandler),
	bg(Background(window)),
	bFactory(BulletFactory(window, 1000, bullets, timeStep)),
	timeStep(timeStep),
	ingameSong(resourceHandler->getSound(ResourceHandler::Sound::INGAME)),
	gameOver(false),
	hardMode(hardMode),
	demo(demo),
	player(std::shared_ptr<Player>(new Player(window, sf::Vector2f(100,250), 10, bFactory, bullets, resourceHandler, timeStep, hardMode)))
{

	if (demo)
	{
		script = resourceHandler->getScript(ResourceHandler::Scripts::GAME_MENU);
	}
	else
	{
		script = resourceHandler->getScriptById(scriptNum);
		// Initialize Background
		bg.addBackground(resourceHandler->getTexture(ResourceHandler::Texture::BACKGROUND1));
		currentScript = scriptNum;
		addObject(std::shared_ptr<Player>(player));
	}

}

void World::playIngameSong() // Workaround <.<
{
	if(!demo)
	{
		ingameSong.play();
		ingameSong.setLoop(true);
	}
}

World::~World()
{
	ingameSong.stop();
	LOGD("World deconstructor called");

}

void World::init(bool demo, int scriptNum)
{

}

void World::process()
{
	// Process loaded script
	bool scriptRunning = script.process(window, objects, bullets, bFactory, resourceHandler, timeStep);
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
					resourceHandler->writeHighScoreScore(player->getPlayerScore() * multiplier, currentScript); // Write highscore
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
	bg.process();


	for (auto &it : bullets)
	{
		it->draw();
	}

	for (auto &it : objects)
	{
		it->draw();
	}

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