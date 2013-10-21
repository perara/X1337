#include "Scene.h"


void Scene::draw(){

	for(auto &it : objects){
		it->draw();
	}

}

void Scene::process(){

	// TODO , how secure is this? 
	for(std::list<Object*>::iterator i = objects.begin(); i != objects.end(); ++i)
	{
		bool upForDeletion = (*i)->process();
		if (!upForDeletion){
			objects.erase(i--); 
		}
	}



	/*for(auto &it : objects){
	if(!it->process()){
	delete it;
	}
	}*/

}


void Scene::addObject(Object* object){
	//LOGD("Object#" << object << " | Object Size: " << this->objects.size());

	//system("pause");
	this->objects.push_back(object);
}
