#include "Scene.h"
#include "Bullet.h"
#include "Shooter.h"


void Scene::draw(){}
void Scene::process(){}

void Scene::setInited(bool init)
{
	this->inited = init;
}

bool Scene::getInited()
{
	return this->inited;
}
