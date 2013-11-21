#include "Object.h"

void Object::draw()
{
	this->window.draw(*this->sprite);
}
bool Object::process()
{
	return true;
}

bool Object::getDeleted(){
	return this->deleted;
}
