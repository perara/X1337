#include "../include/Player.h"
#include "../include/Bullet.h"
#include "../include/BulletFactory.h"
#include "../include/GameShape.h"
#include <memory>
#include <sstream>
#include <iostream>
/// <summary>
/// Initializes a new instance of the <see cref="Player"/> class.
/// </summary>
/// <param name="window">The window.</param>
/// <param name="pos">The position.</param>
/// <param name="radius">The radius.</param>
/// <param name="bFactory">The b factory.</param>
/// <param name="bullets">The bullets.</param>
/// <param name="resourceHandler">The resource handler.</param>
/// <param name="timeStep">The time step.</param>
/// <param name="hardMode">The hard mode.</param>
Player::Player(Renderer& window,
	sf::Vector2f pos,
	int radius,
	BulletFactory& bFactory,
	std::list<std::unique_ptr<Bullet>>& bullets,
	std::shared_ptr<ResourceManager>& resourceHandler,
	const sf::Time& timeStep,
	const bool hardMode,
	std::list<std::shared_ptr<Shooter>>& objects
	):
    Shooter(window, bFactory, bullets, resourceHandler, objects, timeStep),
	playerScore(0),
	playerKills(0),
	radius(radius),
	pulsateGun(false)
{
	// Checks if its hardmode, sets the health correspondingly.
	if (!hardMode)
	{
		setHealth(5);
		startHealth = 5;
	}
	else
	{
		setHealth(1);
		startHealth = 1;
	}

	// Set the shape type
	this->shooterType = Constants::ShooterType::PLAYER;

	// Define the shape for the player
	sprite = std::make_shared<GameShape>(Constants::GameShapeC::Type::PLAYER_SHIP);
	sprite->setTexture(&resourceHandler->getTexture(Constants::ResourceC::Texture::PLAYER_SHIP));
	sprite->setOutlineThickness(1);
	sprite->setOutlineColor(sf::Color::Cyan);
	this->sprite->setPosition(pos);
}

/// <summary>
/// Processes this instance.
/// </summary>
void Player::process()
{
	normalShotTime += timeStep;
	specialShotTime += timeStep;
	pwrUpTime += timeStep;
	pulseTime += timeStep;

	this->hitDetection();
	this->detectEdge();
	this->processPowerUps();

	if (getHealth() <= 0)
	{
		setDeleted(true);
		resourceHandler->getSound(Constants::ResourceC::Sound::FX_ENEMY_DEATH).play();
	}

	this->processShipCollision();


}



/// <summary>
/// This function process a activated power
/// </summary>
void Player::processPowerUps()
{
	// Temporary Pulsegun powerup (lasts 6 seconds)
	if (pulsateGun && pwrUpTime.asSeconds() < 6)
	{

		// Check if the clock  has elapsed 1 second
		if (pulseTime.asMilliseconds() > 1000)
		{

			// Creates a circular bullet pattern.
			for (int i = 1; i < 20; i += 1)
			{

				// Gets a bullet, sets the owner to this, sets the bullet rotation, sets the bullet position, pushes the bullet to the bullet list.
				std::unique_ptr<Bullet> b = getBulletFactory().requestObject(Constants::BulletType::standardShot);
				b->setOwner(this);
				b->setRotation((float)i, sf::Vector2f(150, 150));
				b->sprite->setPosition(this->sprite->getPosition().x, this->sprite->getPosition().y);
				getBullets().push_back(std::move(b));
			}

			pulseTime = sf::milliseconds(0);
		}
	}
	// Time is up! deactivate the pulse gun
	else
	{
		pulsateGun = false;
	}


}

/// <summary>
/// Check if the player hits an edge.
/// </summary>
void Player::detectEdge()
{
	//###################################//
	//####Game Bounds Collision Test#####//
	//###################################//
	//##X AXIS
	if (sprite->getPosition().x - sprite->getRadius() <= 0)
	{
		sprite->setPosition(sprite->getRadius(), sprite->getPosition().y);
	}

	if (sprite->getPosition().x >= renderer.getView().getSize().x - sprite->getRadius())
	{
		sprite->setPosition(renderer.getView().getSize().x - sprite->getRadius(), sprite->getPosition().y);
	}

	//## Y AXIS
	if (sprite->getPosition().y - sprite->getRadius() <= 0)
	{
		sprite->setPosition(sprite->getPosition().x, sprite->getRadius());
	}

	if (sprite->getPosition().y >= renderer.getView().getSize().y - sprite->getRadius())
	{
		sprite->setPosition(sprite->getPosition().x, renderer.getView().getSize().y - sprite->getRadius());
	}
}


/// <summary>
/// Draws the highscore stats, this function runs in the secondary view.
/// </summary>
/// <param name="highScoreList">The high score list as a reference</param>
void Player::drawStats(std::list<HighScoreItem>& highScoreList)
{
	// Draw Background
	sf::RectangleShape barBg;
	barBg.setSize(renderer.getView().getSize());
	barBg.setTexture(&resourceHandler->getTexture(Constants::ResourceC::Texture::PLAYER_BAR));
	barBg.setFillColor(sf::Color(178, 34, 34));
	renderer.draw(barBg);

	// Draw Health
	sf::Text txtHealth;
	txtHealth.setFont(resourceHandler->getFont(Constants::ResourceC::Fonts::SANSATION));
	std::string dead = ((getHealth() <= 0) ? "Dead" : "");
	txtHealth.setString(sf::String(dead));
	txtHealth.setCharacterSize(25);
	txtHealth.setPosition(0, 0);
	txtHealth.setFillColor(sf::Color::White);

	// Sets initial heart location
	float heartY = 0;
    float heartX = 5;

	// Iterate each of the "healths", and display hearts
	for (int i = 1; i <= getHealth(); i++)
	{

		// Draw a heart
		std::shared_ptr<sf::Sprite> sprite = std::make_shared<sf::Sprite>();
		sprite->setTexture(resourceHandler->getTexture(Constants::ResourceC::Texture::HEART), true);
		sprite->setPosition(heartX, heartY);
		sprite->setScale(0.05f, 0.05f);
		renderer.draw(*sprite);

		// Append the width of an haeart to the X position
		heartX += 35;
	}
	renderer.draw(txtHealth);

	// Draw Score
	sf::Text txtScore;
	txtScore.setFont(resourceHandler->getFont(Constants::ResourceC::Fonts::SANSATION));
	txtScore.setString(sf::String("Score: " + std::to_string((int)playerScore)));
	txtScore.setCharacterSize(25);
	txtScore.setPosition(heartX + 10 + txtHealth.getGlobalBounds().width, 0);
	txtScore.setFillColor(sf::Color::White);
	renderer.draw(txtScore);


	// Draw the stage highscore title
	float initY = 0;
	sf::Text txtHighScoreTitle;
	txtHighScoreTitle.setFont(resourceHandler->getFont(Constants::ResourceC::Fonts::SANSATION));
	txtHighScoreTitle.setString(sf::String("HS: "));
	txtHighScoreTitle.setCharacterSize(25);
	txtHighScoreTitle.setPosition(txtScore.getPosition().x + txtScore.getGlobalBounds().width + 10, initY);
	txtHighScoreTitle.setFillColor(sf::Color::White);
	renderer.draw(txtHighScoreTitle);

	// Draw top 1
	if (!highScoreList.empty())
	{
		HighScoreItem& i = highScoreList.front();
		sf::Text txtHighScore;
		txtHighScore.setFont(resourceHandler->getFont(Constants::ResourceC::Fonts::SANSATION));
		txtHighScore.setString(sf::String(i.playerName + ": " + std::to_string((int)i.score)));
		txtHighScore.setCharacterSize(25);
		txtHighScore.setPosition(txtHighScoreTitle.getPosition().x + txtHighScoreTitle.getGlobalBounds().width, initY);
		txtHighScore.setFillColor(sf::Color::White);
		renderer.draw(txtHighScore);
	}



}

/// <summary>
/// Adds to the score.
/// </summary>
/// <param name="score">The score.</param>
void Player::addScore(float score)
{
	playerKills++;
	playerScore += score;
}

/// <summary>
/// Inputs the specified event.
/// </summary>
/// <param name="event">The event.</param>
void Player::input(sf::Event& event)
{
	/*
	// IF COMMA is clicked
	if (event.key.code == sf::Keyboard::Comma && event.type == sf::Event::KeyReleased) // Sound mute button
	{
	std::cout << godMode << std::endl;
	godMode = !godMode;
	}*/

	// Player's left mouse click shoot handler
	if (((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Numpad0) ||
        sf::Mouse::isButtonPressed(sf::Mouse::Left)) &&  normalShotTime.asMilliseconds() > 150){

		// Request a standard bullet and sets the appropriate data
		std::unique_ptr<Bullet> b = getBulletFactory().requestObject(Constants::BulletType::standardShot);
		b->setOwner(this);
		b->sprite->setPosition(sprite->getPosition().x, sprite->getPosition().y - 10);

		// Play shoot sound
		resourceHandler->getSound(Constants::ResourceC::Sound::FX_STANDARD_SHOT).play();

		// Push the bullet to the bullet list
		getBullets().push_back(std::move(b));

		// Restart the clock
		normalShotTime = sf::milliseconds(0);
	}

	// Player's right mouse click shoot handler
	if (((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Numpad1) ||
	        sf::Mouse::isButtonPressed(sf::Mouse::Right)) &&
	        specialShotTime.asMilliseconds() > 1000){

		// Request a heavy shot bullet and set the appropriate data
		std::unique_ptr<Bullet> b = getBulletFactory().requestObject(Constants::BulletType::heavyShot);
		b->setOwner(this);
		b->sprite->setPosition(sprite->getPosition().x, sprite->getPosition().y - 10);

		// Play shoot sound
		resourceHandler->getSound(Constants::ResourceC::Sound::FX_HEAVY_SHOT).play();

		// Push the bullet to the bullet list
		getBullets().push_back(std::move(b));

		// Restart the clock
		specialShotTime = sf::milliseconds(0);
	}

	if (event.type == sf::Event::MouseMoved)
	{
		// Waiting for update: https://github.com/LaurentGomila/SFML/pull/396
		int current_x = sf::Mouse::getPosition(renderer.getWindow()).x;
        int current_y = sf::Mouse::getPosition(renderer.getWindow()).y;
		float elapsed_x = (renderer.getView().getSize().x / 2) - (float)current_x;
        float elapsed_y = (renderer.getView().getSize().y / 2) - (float)current_y;

		if (elapsed_x != 0 || elapsed_y != 0)
		{

			//###################################//
			//#####Mouse Movement Handleing######//
			//###################################//
			//## X axis
			if (sprite->getPosition().x > 0 && sprite->getPosition().x < renderer.getView().getSize().x)
			{
				sprite->move(-elapsed_x, 0);
			}

			// ## Y axis
			if (sprite->getPosition().y > 0 && sprite->getPosition().y < (renderer.getView().getSize().y + renderer.getView().getSize().y))
			{
				sprite->move(0, -elapsed_y);
			}

			sf::Mouse::setPosition(sf::Vector2i(
			        (int)(renderer.getView().getSize().x / 2),
                    (int)(renderer.getView().getSize().y / 2)
                    ), renderer.getWindow());
		}
	}

	if(event.type == sf::Event::KeyPressed ){
        if(event.key.code == sf::Keyboard::Left){
            sprite->move(-2, 0);
        }
        if(event.key.code == sf::Keyboard::Right){
            sprite->move(2, 0);
        }
        if(event.key.code == sf::Keyboard::Up){
            sprite->move(0, -2);
        }
        if(event.key.code == sf::Keyboard::Down){
            sprite->move(0, 2);
        }
	}


}

/// <summary>
/// Gets the player score.
/// </summary>
/// <returns>integer with the player score</returns>
float Player::getPlayerScore() const
{
	return playerScore;
}

/// <summary>
/// Function for activating a power up
/// </summary>
/// <param name="powType">Type of the powwer up to activate</param>
void Player::powerUp(Constants::PowerUpType powType)
{
	// Health Pack Powerup
	if (powType == Constants::PowerUpType::HEALTH_INCREMENT)
	{
		if (getHealth() < getStartHealth())
		{
			setHealth(getHealth() + 1);
		}
	}

	// Pulsating gun Powerup
	else if (powType == Constants::PowerUpType::PULSATING_GUN)
	{
		pulsateGun = true;
		pwrUpTime = sf::milliseconds(0);
		pulseTime = sf::milliseconds(0);
	}

}

int Player::getPlayerKills() const
{
	return playerKills;
}