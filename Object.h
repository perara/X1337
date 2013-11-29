#pragma once
#include <SFML/Window.hpp>
#include <SFML\System\Clock.hpp>
#include <SFML\Graphics\RenderWindow.hpp>
#include <vector>
#include <list>
#include <memory>

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
	Object(sf::RenderWindow & window) : window(window), deleted(false)
	{};

	virtual ~Object();

	virtual void draw();
	virtual void process();
	std::shared_ptr<GameShape> sprite;
	bool getDeleted();

	sf::RenderWindow& window;
protected:
	bool deleted;
};