#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>

class VectorN: public sf::Vector2f
{
public:
	float acceleration;
	int shoot;
	sf::Time sleepTime;
	std::shared_ptr<sf::Sound> emote;

	VectorN() :
		acceleration(0),
		shoot(0),
		sleepTime(sf::milliseconds(0)),
		emote(nullptr)
	{};

};