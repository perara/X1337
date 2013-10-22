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
		std::bind(&World::addBullet, this, std::placeholders::_1)
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


	this->addObject(e1);
	this->addObject(p1);
}


void World::process(){
	for(auto& it : objects){
		it->process();
	}

	// Process Bullets
	for(std::list<Bullet*>::iterator i = std::next(bullets.begin()); i != bullets.end(); ++i)
	{
		bool upForDeletion = (*i)->process();
		if (!upForDeletion){
			this->deleteBullet(i);
		}

		// COLLISION TODO
		for(auto& playerEnemy : objects){ // Loop through all player/enemy objects
			if(CircleTest(*playerEnemy->sprite,  (*(*i)->sprite))){
				this->deleteBullet(i);
			}
		}

	}
}

void World::deleteBullet(std::list<Bullet*>::iterator& i){
	this->bFactory->returnObject( (Bullet*)*i); // TODO CLEAN?
	bullets.erase(i--); 
}

sf::Vector2f World::GetSpriteCenter (const sf::CircleShape& Object)
{
	sf::FloatRect AABB = Object.getGlobalBounds();
	return sf::Vector2f (AABB.left+AABB.width/2.f, AABB.top+AABB.height/2.f);
}

sf::Vector2f World::GetSpriteSize (const sf::CircleShape& Object)
{
	sf::IntRect OriginalSize = Object.getTextureRect();
	sf::Vector2f Scale = Object.getScale();
	return sf::Vector2f (OriginalSize.width*Scale.x, OriginalSize.height*Scale.y);
}

bool World::CircleTest(const sf::CircleShape& Object1, const sf::CircleShape& Object2) {
	sf::Vector2f Obj1Size = GetSpriteSize(Object1);
	sf::Vector2f Obj2Size = GetSpriteSize(Object2);
	float Radius1 = Object1.getRadius();
	float Radius2 = Object2.getRadius();



	sf::Vector2f Distance = GetSpriteCenter(Object1)-GetSpriteCenter(Object2);

	return (Distance.x * Distance.x + Distance.y * Distance.y <= (Radius1 + Radius2) * (Radius1 + Radius2));
}