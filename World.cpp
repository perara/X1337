#include "World.h"
#include "Globals.h"
#include "ResourceHandler.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Background.h"
#include "Log.h"
#include "GameShape.h"

World::World(sf::RenderWindow& window): 
	Scene(window),
	bg(Background(window)),
	bFactory(BulletFactory(window, 1000, bullets)),
	player(Player(window, sf::Vector2f(100,250), 10))
{

}


void World::init()
{
	// Initialize Background
	bg.addBackground(Globals::getInstance().getResourceHandler()->getTexture(ResourceHandler::Texture::BACKGROUND1));

	// Initialize Player
	player.init(this->bFactory, this->bullets);

	if(!this->isDemo()) this->addObject(&player);

	// Set inited to true
	this->setInited(true);
}
void World::init(int scriptNum)
{
	// Initialize Script
	this->script = (Globals::getInstance().getResourceHandler()->getScript((ResourceHandler::Scripts)scriptNum));
	this->init();

}

void World::reset()
{

	// Delete bullets
	for(Bullet* i : this->bullets)
	{
		delete i;
	}
	bullets.clear();

	// Delete objects
	for(Object* i : this->objects)
	{
		delete i;
	}
	objects.clear();

	this->init();

}

bool World::isDemo()
{
	return this->demo;
}

void World::setDemo(bool demo)
{
	this->demo = demo;
	if(this->demo)this->script = (Globals::getInstance().getResourceHandler()->getScript(ResourceHandler::Scripts::GAME_MENU));
}

void World::process()
{
	// Process loaded script
	this->script.process(objects);

	///////////////////////////////////
	// Object processing and cleanup //
	///////////////////////////////////
	if(!objects.empty())
	{
		for(std::list<Shooter*>::iterator i = objects.begin(); i != objects.end();)
		{
			// Init object if its not already inited
			if(!(*i)->getInited()) (*i)->init(bFactory, bullets);

			// Process
			(*i)->process();

			// Cleanup
			if((*i)->getDeleted())
			{ // If the bullet is up for deletion
				delete *i; 
				i = objects.erase(i);
			}
			else
			{
				++i;
			}
		}
	}

	///////////////////////////////////
	// Bullet processing and cleanup //
	///////////////////////////////////
	if(!bullets.empty())
	{
		for(std::list<Bullet*>::iterator it = bullets.begin(); it != bullets.end();)
		{
			// Process
			(*it)->process();

			// Cleanup
			if((*it)->getDeleted())
			{
				(*it)->deleteBullet(bFactory);
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
	player.drawStats();
}

/// <summary>
/// Adds a object to the scene
/// </summary>
/// <param name="object">The object thats requested for addition</param>
void World::addObject(Shooter* object)
{
	//LOGD("Object#" << object << " | Object Size: " << this->objects.size());
	this->objects.push_back(object);
}

void World::addBullet(Bullet* bullet)
{
	//LOGD("Object#" << object << " | Object Size: " << this->objects.size());

	//system("pause");
	this->bullets.push_back(bullet);
}

/// <summary>
/// Draws all scene objects
/// </summary>
void World::draw()
{
	// Draw background
	if(!this->isDemo()) bg.process(); // TODO


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
	this->player.input(event);
}