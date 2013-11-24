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

	this->inputHandler();
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

	if(this->sprite->getPosition().x >= Globals::getInstance().getGameView().getSize().x - this->sprite->getRadius())
	{
		this->sprite->setPosition(Globals::getInstance().getGameView().getSize().x - this->sprite->getRadius(), this->sprite->getPosition().y);
	}

	// Y
	if(this->sprite->getPosition().y - this->sprite->getRadius() <= 0)
	{
		this->sprite->setPosition(this->sprite->getPosition().x, this->sprite->getRadius());
	}


	if(this->sprite->getPosition().y >= Globals::getInstance().getGameView().getSize().y - this->sprite->getRadius())
	{
		this->sprite->setPosition(this->sprite->getPosition().x, Globals::getInstance().getGameView().getSize().y - this->sprite->getRadius());
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

void Player::inputHandler()
{
	/* Shoot handler */
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && clk.getElapsedTime().asMilliseconds() > 100){
		Bullet* b = this->getBulletFactory()->requestObject(BulletFactory::BulletType::standardShot);
		b->setOwner(this);
		b->setPosition(this->sprite->getPosition().x , this->sprite->getPosition().y - 10);
		this->getBullets()->push_back(b);
		clk.restart();
	}

	/* TEMPORARY FOR TESTING */ //TODODODODODODODODODODODOD
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && clk.getElapsedTime().asMilliseconds()){

		Bullet* b = this->getBulletFactory()->requestObject(BulletFactory::BulletType::heavyShot);
		b->setOwner(this);
		b->setPosition(this->sprite->getPosition().x , this->sprite->getPosition().y - 10);
		this->getBullets()->push_back(b);
		clk.restart();
	}

	sf::Event event;
	while (window.pollEvent(event))
	{
		if(event.type == sf::Event::Closed)
		{
			window.close();
		}

		/* Key Events*/
		if(event.type == sf::Event::KeyReleased)
		{
			if(event.key.code == sf::Keyboard::Escape){
				window.close();
			}
		}

		/* Mouse Events */
		if (event.type == sf::Event::MouseButtonPressed)
		{
			//Single click
		}

		if(event.type == sf::Event::MouseMoved)
		{
			int current_x = sf::Mouse::getPosition(window).x, current_y = sf::Mouse::getPosition(window).y;
			int elapsed_x = (Globals::getInstance().getGameView().getSize().x / 2) - current_x, elapsed_y = (Globals::getInstance().getGameView().getSize().y / 2) - current_y;

			if(elapsed_x != 0 || elapsed_y != 0)
			{



				/************************************************************************/
				/* Mouse Movement Handling                                              */
				/************************************************************************/
				if(this->sprite->getPosition().x > 0 && this->sprite->getPosition().x < Globals::getInstance().getGameView().getSize().x)
				{
					this->sprite->move(-elapsed_x  , 0);
				}

				if(this->sprite->getPosition().y > 0 && this->sprite->getPosition().y < (Globals::getInstance().getGameView().getSize().y + Globals::getInstance().getGameView().getSize().y))
				{
					this->sprite->move(0, -elapsed_y);
				}
				sf::Mouse::setPosition(sf::Vector2i((Globals::getInstance().getGameView().getSize().x / 2), (Globals::getInstance().getGameView().getSize().y / 2)), window);
			}
		}
	}
}