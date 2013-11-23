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
	Player* p1 = new Player(
		window, 
		sf::Vector2f(100,250), 
		10);
	p1->init(this->bFactory, this->bullets);

	// Add player objects
	std::queue<sf::Vector3f>* pathQueue = new std::queue<sf::Vector3f>();
	pathQueue->push(sf::Vector3f(250,50,0));
	pathQueue->push(sf::Vector3f(250,250,0));
	pathQueue->push(sf::Vector3f(250,50,0));
	pathQueue->push(sf::Vector3f(250,50,0));
	pathQueue->push(sf::Vector3f(250,250,0));
	Enemy* e1 = new Enemy(
		window, 
		pathQueue,
		10);
	e1->init(this->bFactory, this->bullets);

	//this->addObject(e1);
	this->addObject(p1);

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
				
				delete *i; 
				objects.erase(i);
				i = objects.begin();

			}
		}
	}

}