#pragma once
#include <SFML\Graphics.hpp>

/// <summary>
/// The scene class is an abstract class which is base class for all States. All state is a Scene.
/// </summary>
class Scene
{
public:
	Scene(sf::RenderWindow& window):window(window){};

	virtual void draw() = 0;
	virtual void process() = 0;
	virtual void init() = 0;
	virtual void input(sf::Event&) = 0;
	virtual void reset() = 0; //Should also call setInit(false)

	void setInited(bool);
	bool getInited();

protected:
	sf::RenderWindow& window;
	bool inited;
};