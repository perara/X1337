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
	Object(sf::RenderWindow & window) : window(window), deleted(false){};
	virtual ~Object();

	// Delete getter/setter
	bool getDeleted();
	void setDeleted(bool);

	std::shared_ptr<GameShape> sprite;

	virtual void draw();
	virtual void process();
protected:
	bool isOutOfBounds();

	sf::RenderWindow& window;

	// Hyperplane separation theorem for hit detection
	bool sat(std::shared_ptr<GameShape> c1, std::shared_ptr<GameShape> c2);
private:
	bool deleted;
};
