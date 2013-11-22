#include "World.h"
#include "BulletFactory.h";
#include "Globals.h"
#include "ResourceHandler.h"
#include "Player.h"
#include <queue>
#include "Enemy.h"

sf::Clock clkW;
int count = 0;

World::World(sf::RenderWindow& window): Scene(window)
{
	// Initialize Background
	bg->addBackground(Globals::getInstance().getResourceHandler()->getTexture(ResourceHandler::Texture::BACKGROUND1));

	// Initialize Factories
	bFactory = new BulletFactory(window, 1000, bullets);

	// Add player objects
	Player* p1 = new Player(
		window, 
		sf::Vector2f(100,250), 
		10, 
		bFactory,
		bullets
		);

	// Add player objects
	std::queue<sf::Vector3f> pathQueue;
	pathQueue.push(sf::Vector3f(50,50,0));
	pathQueue.push(sf::Vector3f(250,250,0));
	pathQueue.push(sf::Vector3f(30,30,0));

	Enemy* e1 = new Enemy(
		window, 
		pathQueue,
		10, 
		bFactory,
		bullets
		);
	
	this->addObject(e1);
	this->addObject(p1);
}

void World::process()
{
	/* Processing Processing */
	for(auto& it : objects)
	{
		it->process();
	}

	for(auto& it : bullets)
	{
		it->process();
	}

	

	/* Cleanup Processing */
	if(!bullets.empty())
	{
		for(std::list<Bullet*>::iterator i = bullets.begin(); i != bullets.end();i++)
		{
			if((*i)->getDeleted())
			{ // If the bullet is up for deletion
				(*i)->deleteBullet(i);
				if(bullets.empty()) break;
				i = bullets.begin();
			}
		}
	}

	/* Cleanup Processing OBJECTS */
	if(!objects.empty())
	{
		for(std::list<Object*>::iterator i = objects.begin(); i != objects.end();i++)
		{
			if((*i)->getDeleted())
			{ // If the bullet is up for deletion
				if(objects.empty()) break;
				objects.erase(i);
				i = objects.begin();
			}
		}
	}




	if(clkW.getElapsedTime().asMilliseconds() > 100)
	{
		count++;
		// Add player objects
		/*Enemy* e1 = new Enemy(
			window, 
			sf::Vector2f(250,10), 
			sf::Vector2f(rand() % 500 + 10,rand() % 500 + 10), 
			50,
			10, 
			bFactory,
			bullets
			);*/
		
		//this->addObject(e1);
		clkW.restart();
	}

}