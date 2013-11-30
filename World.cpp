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
			 const sf::Time& timeStep):
Scene(window, resourceHandler),
	bg(Background(window)),
	bFactory(BulletFactory(window, 1000, bullets, timeStep)),
	player(std::shared_ptr<Player>(new Player(window, sf::Vector2f(100,250), 10, bFactory, bullets, resourceHandler, timeStep))),
	timeStep(timeStep),
	ingameSong(resourceHandler->getSound(ResourceHandler::Sound::INGAME)),
	gameOver(false),
	hardMode(false)
{

}

World::~World()
{
	ingameSong.stop();
	LOGD("World deconstructor called");

}

void World::init(bool demo, int scriptNum)
{
	if(!hardMode)
		player->setHealth(5);
	else
		player->setHealth(0);

	this->demo = demo;
	if(demo)
	{
		this->script = resourceHandler->getScript(ResourceHandler::Scripts::GAME_MENU);
	}
	else
	{
		this->script = resourceHandler->getScriptById(scriptNum);
		ingameSong.play();
		ingameSong.setLoop(true);
		// Initialize Background
		bg.addBackground(resourceHandler->getTexture(ResourceHandler::Texture::BACKGROUND1));
		currentScript = scriptNum;
		addObject(std::shared_ptr<Player>(player));
	}

}

void World::process()
{
	// Process loaded script
	bool scriptRunning = this->script.process(window, objects, bullets, bFactory, resourceHandler, timeStep);

	///////////////////////////////////
	// Object processing and cleanup //
	///////////////////////////////////
	if(!objects.empty() || scriptRunning)
	{
		for(std::list<std::shared_ptr<Shooter>>::iterator i = objects.begin(); i != objects.end();)
		{
			// Process
			(*i)->process();

			// Cleanup
			if((*i)->getDeleted())
			{ // If the bullet is up for deletion
				if((*i)->getType() == Shooter::ShooterType::ENEMY)
				{
					player->addScore((*i)->getValue());
				}
				else if((*i)->getType() == Shooter::ShooterType::PLAYER)
				{
					gameOver = true;
					resourceHandler->writeHighScoreScore(player->getPlayerScore(), currentScript); // Write highscore
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
	}

	///////////////////////////////////
	// Bullet processing and cleanup //
	///////////////////////////////////
	if(!bullets.empty())
	{
		for(std::list<std::unique_ptr<Bullet>>::iterator it = bullets.begin(); it != bullets.end();)
		{
			// Process
			(*it)->process();

			// Cleanup
			if((*it)->getDeleted())
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

void World::setHardMode(bool hardOn)
{
	hardMode=hardOn;
}
bool World::getHardMode()
{
	return hardMode;
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


	for(auto &it : bullets)
	{
		it->draw();
	}

	for(auto &it : objects)
	{
		it->draw();

	}
}

void World::input(sf::Event& event)
{
	this->player->input(event);
}