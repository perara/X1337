#include "Scene.h"


/// <summary>
/// Draws all scene objects
/// </summary>
void Scene::draw(){

	for(auto &it : objects){
		it->draw();
	}

}

/// <summary>
/// Processes all scene objects
/// </summary>
void Scene::process(){

	// TODO , how secure is this? 
	for(std::list<Object*>::iterator i = objects.begin(); i != objects.end(); ++i)
	{
		bool upForDeletion = (*i)->process();
		if (!upForDeletion){
			objects.erase(i--); 
		}
	}

}


/// <summary>
/// Adds a object to the scene
/// </summary>
/// <param name="object">The object thats requested for addition</param>
void Scene::addObject(Object* object){
	//LOGD("Object#" << object << " | Object Size: " << this->objects.size());

	//system("pause");
	this->objects.push_back(object);
}
