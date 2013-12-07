#pragma once
#include <SFML\Graphics\RenderWindow.hpp>
#include <memory>

class GameShape;
/// <summary>
/// Abstract base class for all objects in the game. A object can for example be a Player, Bullet or a Enemy.
/// </summary>
class Object
{
public:
	Object(sf::RenderWindow & window) : window(window), deleted(false)
	{};

	virtual ~Object();

	virtual void draw();
	virtual void process();
	std::shared_ptr<GameShape> sprite;
	bool getDeleted();
	void setDeleted(bool);
	bool isOutOfBounds();

	sf::RenderWindow& window;
protected:
	bool deleted;

	// Hyperplane separation theorem for hit detection
	bool sat(std::shared_ptr<GameShape> c1, std::shared_ptr<GameShape> c2);
};