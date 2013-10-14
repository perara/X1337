#include "Scene.h"


void Scene::draw(sf::RenderWindow &window){

	for(auto &it : objects){
		it->draw(window);
	}

}

void Scene::process(){


}

void Scene::addObject(Object* object){
	this->objects.push_back(object);
}
