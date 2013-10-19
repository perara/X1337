#include "Scene.h"


void Scene::draw(){

	for(auto &it : objects){
		it->draw();
	}

}

void Scene::process(){
	for(auto &it : objects){
		it->process();
	}
}



void Scene::addObject(Object* object){
	this->objects.push_back(object);
}
