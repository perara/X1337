#include "World.h"
#include "Enemy.h"


World::World(sf::RenderWindow& window): Scene(window){
	// Initialize Factories
	bFactory = new BulletFactory(window, 1000);

	// Add player objects
	Player* p1 = new Player(
		window, 
		sf::Vector2f(100,250), 
		10, 
		bFactory,
		std::bind(&World::addObject, this, std::placeholders::_1)
		);

	// Add player objects
	Enemy* e1 = new Enemy(
		window, 
		sf::Vector2f(250,50), 
		sf::Vector2f(250,300), 
		50,
		10, 
		bFactory,
		std::bind(&World::addObject, this, std::placeholders::_1)
		);



	this->addObject(p1);
}


void World::process(){
	// TODO , how secure is this? 
	for(std::list<Object*>::iterator i = objects.begin(); i != objects.end(); ++i)
	{
		bool upForDeletion = (*i)->process();
		if (!upForDeletion){
			this->bFactory->returnObject( (Bullet*)*i); // TODO CLEAN?
			objects.erase(i--); 

		}
	}
}
