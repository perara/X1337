#include "Object.h"
#include "Log.h"
#include "GameShape.h"

void Object::draw()
{
	this->window.draw(*this->sprite);

	/*sf::RectangleShape();
	sf::FloatRect bounds = this->sprite->getGlobalBounds();
	sf::RectangleShape af(sf::Vector2f(bounds.width,bounds.height));
	af.setPosition(bounds.left, bounds.top);
	af.setOutlineColor(sf::Color(141,23,22,23));
	af.setFillColor(sf::Color(255,255,255,150));

	this->window.draw(af);*/

}
void Object::process(){}

bool Object::getDeleted(){
	return this->deleted;
}

void Object::setDeleted(bool del)
{
	deleted = del;
}


Object::~Object()
{

}
