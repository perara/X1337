#include "Player.h"
#include "Bullet.h"
#include "BulletFactory.h"
#include "GameShape.h"
#include "Log.h"
#include "Globals.h"
#include "ResourceHandler.h"

sf::Clock clk; //TESTING PURPOSES

/// <summary>
/// Initializes a new instance of the <see cref="Player"/> class.
/// </summary>
/// <param name="window">The render window.</param>
/// <param name="pos">The initial startposition of the player</param>
/// <param name="radius">The radius.</param>
/// <param name="bFactory">The <see cref=BulletFactory"></param>
/// <param name="sceneBulletListCallback">The scene object call back. This is basicly a function pointer to the corresponding world function "addObject" Reason for passing this is so we can add bullets to the Scene loop</param>
Player::Player(sf::RenderWindow& window, sf::Vector2f pos, int radius):
	Shooter(window)
{
	this->setType(Shooter::ShooterType::PLAYER);
	this->sprite = new GameShape(GameShape::CIRCLE, 10);
	this->sprite->setPosition(pos);
}

void Player::process()
{
	if(!this->getInited()) return;

	this->shooterProcess();
	this->detectEdge();


}

void Player::detectEdge()
{
	/************************************************************************/
	/* Game Bounds Collision test                                           */
	/************************************************************************/

	// X
	if(this->sprite->getPosition().x - this->sprite->getRadius() <= 0)
	{
		this->sprite->setPosition(this->sprite->getRadius(), this->sprite->getPosition().y);
	}

	if(this->sprite->getPosition().x >= window.getSize().x - this->sprite->getRadius())
	{
		this->sprite->setPosition(window.getSize().x - this->sprite->getRadius(), this->sprite->getPosition().y);
	}

	// Y
	if(this->sprite->getPosition().y - this->sprite->getRadius() <= 0)
	{
		this->sprite->setPosition(this->sprite->getPosition().x, this->sprite->getRadius());
	}


	if(this->sprite->getPosition().y >= window.getSize().y - this->sprite->getRadius())
	{
		this->sprite->setPosition(this->sprite->getPosition().x, window.getSize().y - this->sprite->getRadius());
	}
}

void Player::drawStats()
{
	// Draw Health
	sf::Text txtHealth;
	txtHealth.setFont(Globals::getInstance().getResourceHandler()->getFont(ResourceHandler::Fonts::SANSATION));
	txtHealth.setString(sf::String("Health: "));
	txtHealth.setCharacterSize(25);
	txtHealth.setPosition(20,20);
	txtHealth.setColor(sf::Color::White);

	int heartX = 105;
	for(int i = 0; i < this->getHealth(); i++)
	{

		std::shared_ptr<sf::Sprite> sprite = std::shared_ptr<sf::Sprite>(new sf::Sprite);
		sprite->setTexture(Globals::getInstance().getResourceHandler()->getTexture(ResourceHandler::Texture::HEART), true);
		sprite->setPosition(heartX, 20);
		sprite->setScale(0.05f,0.05f);
		this->window.draw(*sprite);
		heartX = heartX + 35;
	}

	this->window.draw(txtHealth);


}

void Player::input(sf::Event& event)
{
	/* Shoot handler */
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && clk.getElapsedTime().asMilliseconds() > 100){
		Bullet* b = this->getBulletFactory()->requestObject(Bullet::Type::standardShot);
		b->setOwner(this->getType());
		b->setPosition(this->sprite->getPosition().x , this->sprite->getPosition().y - 10);
		this->getBullets()->push_back(b);
		Globals::getInstance().getResourceHandler()->getSound(ResourceHandler::Sound::STANDARD_SHOT).play();
		clk.restart();
	}

	/* TEMPORARY FOR TESTING */ //TODODODODODODODODODODODOD
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && clk.getElapsedTime().asMilliseconds()){

		Bullet* b = this->getBulletFactory()->requestObject(Bullet::Type::heavyShot);
		b->setOwner(this->getType());
		b->setPosition(this->sprite->getPosition().x , this->sprite->getPosition().y - 10);
		this->getBullets()->push_back(b);
		Globals::getInstance().getResourceHandler()->getSound(ResourceHandler::Sound::HEAVY_SHOT).play();
		clk.restart();
	}




	if(event.type == sf::Event::MouseMoved)
	{
		int current_x = sf::Mouse::getPosition(window).x, current_y = sf::Mouse::getPosition(window).y;
		int elapsed_x = (window.getSize().x / 2) - current_x, elapsed_y = (window.getView().getSize().y / 2) - current_y;

		if(elapsed_x != 0 || elapsed_y != 0)
		{

			/************************************************************************/
			/* Mouse Movement Handling                                              */
			/************************************************************************/
			if(this->sprite->getPosition().x > 0 && this->sprite->getPosition().x < window.getSize().x)
			{
				this->sprite->move(-elapsed_x  , 0);
			}

			if(this->sprite->getPosition().y > 0 && this->sprite->getPosition().y < (window.getSize().y + window.getSize().y))
			{
				this->sprite->move(0, -elapsed_y);
			}
			sf::Mouse::setPosition(sf::Vector2i((window.getSize().x / 2), (window.getSize().y / 2)), window);
		}
	}
}