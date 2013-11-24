#include "World.h"
#include "BulletFactory.h";
#include "Globals.h"
#include "ResourceHandler.h"
#include "Player.h"
#include <queue>
#include "Enemy.h"
#include "Bullet.h"


sf::Clock clkW;
int count = 0;

World::World(sf::RenderWindow& window): Scene(window)
{
	// Initialize Script
	this->setScript(Globals::getInstance().getResourceHandler()->getScript(ResourceHandler::Scripts::ENCOUNTER3));

	// Initialize Background
	bg->addBackground(Globals::getInstance().getResourceHandler()->getTexture(ResourceHandler::Texture::BACKGROUND1));

	// Initialize Factories
	this->bFactory = new BulletFactory(window, 1000, bullets);
	
	// Add player objects
	this->player = new Player(
		window, 
		sf::Vector2f(100,250), 
		10);
	player->init(this->bFactory, this->bullets);
	this->addObject(player);

}

void World::process()
{
	// Process the script
	this->getScript()->process(objects);

	/* Processing Processing */
	for(auto& it : objects)
	{
		if(!it->getInited()) it->init(bFactory, bullets);

		it->process();
	}

	for(auto& it : bullets)
	{
		it->process();
	}



	/* Cleanup Bullets  TODO*/
	if(!bullets.empty())
	{
		/*	for(std::list<Bullet*>::iterator i = bullets.begin(); i != bullets.end();i++)
		{
		if((*i)->getDeleted())
		{ // If the bullet is up for deletion
		(*i)->deleteBullet();
		bullets.erase(i);
		if(bullets.empty()) break;
		i = bullets.begin();
		}
		}*/

		std::vector<Bullet*> tmp;
		for(auto& i : bullets)
		{
			if(i->getDeleted())
			{
				i->deleteBullet();
			}else
			{
				tmp.push_back(i);
			}

		}
		bullets.clear(); // Needed?
		bullets = tmp;

	}


	/* Cleanup Processing OBJECTS */
	if(!objects.empty())
	{
		for(std::vector<Shooter*>::iterator i = objects.begin(); i != objects.end();i++)
		{

			if((*i)->getDeleted())
			{ // If the bullet is up for deletion
				if(objects.empty()) break;

				delete *i; 
				objects.erase(i);
				i = objects.begin();

			}
		}
	}

}

void World::drawStats()
{
	player->drawStats();


}