#include "Object.h"
#include <SFML/Window.hpp>


void Object::draw(sf::RenderWindow &window){
	window.draw(*this->sprite);
}

void Object::process(){
	
}

