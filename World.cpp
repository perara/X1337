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
	sliceEngine(SliceEngine<Bullet*>(sf::FloatRect(0,0, window.getSize().x,window.getSize().y), 500)),
	bg(Background(window)),
	bFactory(BulletFactory(window, 1000, bullets)),
	player(Player(window, sf::Vector2f(100,250), 10))
{
	this->init();
}



void World::init()
{
	// Initialize Script
	this->setScript(Globals::getInstance().getResourceHandler()->getScript(ResourceHandler::Scripts::ENCOUNTER3));

	// Initialize Background
	bg.addBackground(Globals::getInstance().getResourceHandler()->getTexture(ResourceHandler::Texture::BACKGROUND1));

	// Initialize Player
	player.init(this->bFactory, this->bullets);

	if(!this->isDemo()) this->addObject(&player);

	// Set inited to true
	this->setInited(true);
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
	if(this->demo)this->setScript(Globals::getInstance().getResourceHandler()->getScript(ResourceHandler::Scripts::GAME_MENU));
}

void World::process()
{
	// Process loaded script
	this->getScript()->process(objects);

	///////////////////////////////////
	// Object processing and cleanup //
	///////////////////////////////////
	if(!objects.empty())
	{
		for(std::vector<Shooter*>::iterator i = objects.begin(); i != objects.end();i++)
		{
			// Init object if its not already inited
			if(!(*i)->getInited()) (*i)->init(bFactory, bullets);

			// Process
			(*i)->process();

			// Cleanup
			if((*i)->getDeleted())
			{ // If the bullet is up for deletion
				if(objects.empty()) break;

				delete *i; 
				objects.erase(i);
				i = objects.begin();

			}
		}
	}

	///////////////////////////////////
	// Bullet processing and cleanup //
	///////////////////////////////////
	if(!bullets.empty())
	{
		std::vector<Bullet*> tmp;
		for(auto& it : bullets)
		{

			// Process
			it->process();

			// Cleanup
			if(it->getDeleted())
			{
				it->deleteBullet(bFactory);
			}else
			{
				tmp.push_back(it);
			}

		}
		bullets.clear(); // Needed?
		bullets = tmp;
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

Script* World::getScript()
{
	if(!this->script == NULL)
		return this->script;
	else{
		LOGE("Script was loaded unsuccessfully (nullptr)");
		return NULL;
	}
}

void World::setScript(Script* script)
{
	this->script = script;
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