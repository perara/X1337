#include "World.h"
#include "BulletFactory.h";
#include "Globals.h"
#include "ResourceHandler.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Background.h"
#include "Log.h"

#include "GameShape.h"

World::World(sf::RenderWindow& window): Scene(window)
{
	this->init();
}



void World::init()
{
	// Initialize Script
	this->setScript(Globals::getInstance().getResourceHandler()->getScript(ResourceHandler::Scripts::ENCOUNTER3));

	// Initialize Background
	bg = new Background(window);
	bg->addBackground(Globals::getInstance().getResourceHandler()->getTexture(ResourceHandler::Texture::BACKGROUND1));

	// Initialize Bullet Factory
	this->bFactory = new BulletFactory(window, 1000, bullets);

	// Initialize Player
	this->player = new Player(
		window, 
		sf::Vector2f(100,250), 
		10);
	player->init(this->bFactory, this->bullets);
	if(!this->isDemo()) this->addObject(player);

	// Set inited to true
	this->setInited(true);
}

void World::reset()
{
	if(!(this->bg != nullptr))delete this->bg;
	if(!(this->bFactory != nullptr))delete bFactory;
	if(!(this->player != nullptr))delete this->player;

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
	// Process the script
	this->getScript()->process(objects);


	/* Cleanup Bullets  TODO*/
	if(!bullets.empty())
	{

		for(auto& it : bullets)
		{
			it->process();
		}


		std::vector<Bullet*> tmp;
		for(auto& i : bullets)
		{
			if(i->getDeleted())
			{
				i->deleteBullet(*bFactory);
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
		/* Processing Processing */
		for(auto& it : objects)
		{
			if(!it->getInited()) it->init(bFactory, bullets);

			it->process();
		}

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
	if(!this->isDemo()) bg->process(); // TODO

	for(auto &it : objects)
	{
		it->draw();


		/* TODO REMOVE LATER*/
		/*sf::Text* txt = new sf::Text();
		txt->setFont(Globals::getInstance().getResourceHandler()->getFont(ResourceHandler::Fonts::SANSATION));

		int number = it->getHealth();
		std::stringstream ss;//create a stringstream
		ss << number;//add number to the stream
		txt->setString(sf::String(ss.str()));
		txt->setCharacterSize(15);
		txt->setPosition(it->sprite->getGlobalBounds().left, it->sprite->getGlobalBounds().top);
		txt->setColor(sf::Color::Red);
		window.draw(*txt);*/
	}

	for(auto &it : bullets)
	{
		it->draw();
	}
}

void World::input(sf::Event& event)
{
	this->player->input(event);
}