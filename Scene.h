#pragma once
#include "Object.h"
#include "Log.h"
#include "Bullet.h"

#include <list>

/// <summary>
/// The scene class is an abstract class which is base class for all States. All state is a Scene.
/// </summary>
class Scene{
public:
	std::list<Object*> objects; // Reason for using List here would be to not invalidate iterator on insertion. Need input
	std::list<Bullet*> bullets;
	Scene(sf::RenderWindow& window):window(window){}
	virtual void draw();
	virtual void process();
	void addObject(Object* object);
	void addBullet(Bullet* bullet);

protected:
	sf::RenderWindow & window;
};