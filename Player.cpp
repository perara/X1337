#include "Player.h"

void Player::setPlayerPosX(float x)
{
	playerShape.setPosition(x, playerShape.getPosition().y);
}
void Player::setPlayerPosY(float y)
{
	playerShape.setPosition(playerShape.getPosition().x, y);
}
Player::Player(sf::Vector2f loc, int rad)
{
	playerShape.setRadius(rad);
	playerShape.setPosition(loc.x, loc.y);
}

Player::Player(void)
{
}


Player::~Player(void)
{
}
