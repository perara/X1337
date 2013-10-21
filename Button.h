#pragma once
#include <SFML\Graphics\RectangleShape.hpp>
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\Text.hpp>
#include <string>

/// <summary>
/// A button class for menu //TODO
/// </summary>
class Button: public sf::RectangleShape{
public:
	Button(sf::String label,int labelSize, sf::Vector2f position, sf::Vector2f size);
	bool isClicked();
	void Draw(sf::RenderWindow &window);

private:
	sf::Text label;
	sf::FloatRect hitBox;

};