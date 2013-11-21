#include "Button.h"

Button::Button(sf::String label, int labelSize, sf::Vector2f position, sf::Vector2f size)
{
	// Initialize the Button Size and properties
	this->setPosition(position);
	this->setSize(size);
	this->setOutlineColor(sf::Color::Red);
	this->setOutlineThickness(5);

	// Create a hitBox
	this->hitBox = sf::FloatRect(this->getGlobalBounds());

	//Initialize the Label and set corresponding positions
	//sf::Text lbl(label, Config::getInstance().font, labelSize);
	//lbl.setPosition(position);
	//lbl.setColor(sf::Color::Black);

	// Ensures the text dont go outside the button, downscaling it.
	/*while(lbl.findCharacterPos(label.getSize()).x >= this->getPosition().x + this->getSize().x){
		lbl.setCharacterSize(lbl.getCharacterSize()-1);
	}*/
	// Add label to button
	//this->label = lbl;
}

// This function checks if the button is clicked, This is the equavilent to process
bool Button::isClicked()
{
	sf::RectangleShape mPos(sf::Vector2f(1,1));
	mPos.setPosition(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->hitBox.intersects(mPos.getGlobalBounds())){
		std::cout << "Hello i clicked" << std::endl;
	}
	return true;
}

void Button::Draw(sf::RenderWindow &window)
{
	window.draw(*this);
	window.draw(this->label);
}
