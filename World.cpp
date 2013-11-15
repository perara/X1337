#include "World.h"

World::World(sf::RenderWindow& window): Scene(window){
	// Initialize Factories
	bFactory = new BulletFactory(window, 1000);


	// Add player objects
	Player* p1 = new Player(
		window, 
		sf::Vector2f(100,250), 
		10, 
		bFactory,
		bullets
		);

	// Add player objects
	Enemy* e1 = new Enemy(
		window, 
		sf::Vector2f(250,50), 
		sf::Vector2f(250,300), 
		50,
		10, 
		bFactory,
		bullets
		);


	this->addObject(e1);
	this->addObject(p1);
}


void World::process(){
	for(auto& it : objects){
		it->process();
	}
}
