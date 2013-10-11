#include <SFML\Graphics.hpp>
#pragma once
class Player
{
public:
	
	sf::CircleShape playerShape;
	void setPlayerPosX(float x);
	void setPlayerPosY(float x);
	Player(sf::Vector2f loc, int rad);
	Player(void);
	~Player(void);
};

