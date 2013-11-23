#pragma once
#include "Log.h"
#include "Background.h"

class Bullet;
class Object;
class BulletFactory;
class Script;

#include <list>

/// <summary>
/// The scene class is an abstract class which is base class for all States. All state is a Scene.
/// </summary>
class Scene
{
public:
	std::list<Object*> objects; 
	std::list<Bullet*> bullets;
	BulletFactory* bFactory;

	Scene(sf::RenderWindow& window):window(window)
	{
		bg = new Background(window);
		script = NULL;

	}

	virtual void draw();
	virtual void process();
	void addObject(Object* object);
	void addBullet(Bullet* bullet);

protected:
	sf::RenderWindow & window;
	Background* bg;

	Script* getScript();
	void setScript(Script*);

private:
	Script * script;
};