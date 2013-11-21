#include "Player.h"
sf::Clock clk; //TESTING PURPOSES

/// <summary>
/// Initializes a new instance of the <see cref="Player"/> class.
/// </summary>
/// <param name="window">The render window.</param>
/// <param name="pos">The initial startposition of the player</param>
/// <param name="radius">The radius.</param>
/// <param name="bFactory">The <see cref=BulletFactory"></param>
/// <param name="sceneBulletListCallback">The scene object call back. This is basicly a function pointer to the corresponding world function "addObject" Reason for passing this is so we can add bullets to the Scene loop</param>
Player::Player(sf::RenderWindow& window, sf::Vector2f pos, int radius, BulletFactory* bFactory, std::list<Bullet*>& bullets):
	Shooter(window, bullets, bFactory)
{
	this->sprite = new GameShape(GameShape::circle, 10);
	this->sprite->setPosition(pos);
}

bool Player::process()
{
	this->inputHandler();
	this->shootableProcess();
	return true;
}

void Player::inputHandler()
{
	/* Shoot handler */
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && clk.getElapsedTime().asMilliseconds() > 100){
		Bullet* b = this->bFactory->requestObject(BulletFactory::BulletType::standardShot);
		b->setOwner(this);
		b->setPosition(this->sprite->getPosition().x , this->sprite->getPosition().y - 10);
		this->bullets.push_back(b);
		clk.restart();
	}

	/* TEMPORARY FOR TESTING */ //TODODODODODODODODODODODOD
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && clk.getElapsedTime().asMilliseconds() > 100){

		Bullet* b = this->bFactory->requestObject(BulletFactory::BulletType::heavyShot);
		b->setOwner(this);
		b->setPosition(this->sprite->getPosition().x , this->sprite->getPosition().y - 10);
		this->bullets.push_back(b);
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
			int elapsed_x = (window.getSize().x / 2) - current_x, elapsed_y = (window.getSize().y / 2) - current_y;

			if(elapsed_x != 0 || elapsed_y != 0)
			{

				/************************************************************************/
				/* Special Cases where user gets stuck                                                                  */
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

				/************************************************************************/
				/* Mouse Movement Handling                                              */
				/************************************************************************/
				if(this->sprite->getPosition().x > 0 && this->sprite->getPosition().x < window.getSize().x)
				{
					this->sprite->move(-elapsed_x  , 0);
				}

				if(this->sprite->getPosition().y > 0 && this->sprite->getPosition().y < (window.getPosition().y + window.getSize().y))
				{
					this->sprite->move(0, -elapsed_y);
				}
				sf::Mouse::setPosition(sf::Vector2i((window.getSize().x / 2), (window.getSize().y / 2)), window);
			}
		}
	}
}


int Player::hitDetection(){
	return 0;
}