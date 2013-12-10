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
	/// Constructor for the object which sets the corresponding deleted variable to false as default
	Object(sf::RenderWindow & window) : window(window), deleted(false){};
	virtual ~Object();

	// Delete getter/setter
	bool getDeleted();
	void setDeleted(bool);

	/// GameShape pointer which defines an sprite for a object.
	std::shared_ptr<GameShape> sprite;

	virtual void draw();
	virtual void process() = 0;
protected:
	bool isOutOfBounds();

	/// sf::Renderwindow reference which originate from GameEngine
	sf::RenderWindow& window;

	/// Hyperplane separation theorem for hit detection
	bool sat(std::shared_ptr<GameShape> c1, std::shared_ptr<GameShape> c2);
private:
	/// Boolean which describes if the Object is up for/is deleted.
	bool deleted;
};
