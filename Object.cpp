#include "Object.h"
#include <SFML/Window.hpp>
#include <iostream>

void Object::draw(){
	this->window.draw(*this->sprite);
}

bool Object::process(){
	
	return true;
}

