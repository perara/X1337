#include "Scene.h"

/// <summary>
/// Draws all scene objects
/// </summary>
void Scene::draw()
{
	// Draw background
	bg->process();

	for(auto &it : objects)
	{
		it->draw();
	}

	for(auto &it : bullets)
	{
		it->draw();
	}
}

/// <summary>
/// Processes all scene objects
/// </summary>
void Scene::process()
{

}

/// <summary>
/// Adds a object to the scene
/// </summary>
/// <param name="object">The object thats requested for addition</param>
void Scene::addObject(Object* object)
{
	//LOGD("Object#" << object << " | Object Size: " << this->objects.size());

	//system("pause");
	this->objects.push_back(object);
}

void Scene::addBullet(Bullet* bullet)
{
	//LOGD("Object#" << object << " | Object Size: " << this->objects.size());

	//system("pause");
	this->bullets.push_back(bullet);
}