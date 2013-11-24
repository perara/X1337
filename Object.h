#pragma once
#include <SFML/Window.hpp>
#include <SFML\System\Clock.hpp>
#include <SFML\Graphics\RenderWindow.hpp>
#include <vector>
#include <list>

class GameShape;
class BulletFactory;
class Bullet;

/// <summary>
/// Abstract base class for all objects in the game. A object can for example be a Player, Bullet or a Enemy.
/// </summary>
class Object
{
public:
	Object();
	Object(sf::RenderWindow & window) : window(window)
	{
		deleted = false;
		inited = false;
	};

	virtual ~Object();

	virtual void draw();
	virtual void process();
	GameShape* sprite;
	bool getDeleted();
	bool getInited();
	void setInited(bool);
	virtual void init(BulletFactory*, std::vector<Bullet*>&);

	// Bullets getter/setter
	std::vector<Bullet*>* getBullets();
	void setBullets(std::vector<Bullet*>& bullets);

	// BulletFactory Getter/Setter
	BulletFactory* getBulletFactory();
	void setBulletFactory(BulletFactory* bFactory);

protected:
	sf::RenderWindow& window;
	bool deleted;
	bool inited;

	BulletFactory* bFactory;
	std::vector<Bullet*>* bullets;
};