#include "Object.h"

void Object::draw()
{
	this->window.draw(*this->sprite);
}
void Object::process(){}

bool Object::getDeleted(){
	return this->deleted;
}
