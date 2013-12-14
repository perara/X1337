#include "Player.h"
#include "Bullet.h"
#include "BulletFactory.h"
#include "GameShape.h"
#include "Log.h"
#include "ResourceHandler.h"
#include <sstream>

/// <summary>
/// The normal shot clock
/// </summary>
sf::Clock normalShotClock;

/// <summary>
/// The special shot clock
/// </summary>
sf::Clock specialShotClock;

/// <summary>
/// The score time
/// </summary>
float scoreTime;

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
Player::Player(sf::RenderWindow& window,
			   sf::Vector2f pos,
			   int radius, BulletFactory& bFactory,
			   std::list<std::unique_ptr<Bullet>>& bullets,
			   std::shared_ptr<ResourceHandler>& resourceHandler,
			   const sf::Time& timeStep,
			   const bool hardMode
			   )
			   :
playerScore(0),
	pulsateGun(false),

	Shooter(window, bFactory, bullets, resourceHandler, timeStep)
{

	// Checks if its hardmode, sets the health correspondingly.
	if (!hardMode)
	{
		setHealth(5);
	}
	else
	{
		setHealth(1);
	}

	// Set the shape type
	this->shooterType = Shooter::ShooterType::PLAYER;

	// Define the shape for the player
	sprite = std::shared_ptr<GameShape>(new GameShape(GameShape::ShapeType::PLAYER_SHIP));
	sprite->setTexture(&resourceHandler->getTexture(ResourceHandler::Texture::PLAYER_SHIP));
	sprite->setOutlineThickness(1);
	sprite->setOutlineColor(sf::Color::Cyan);
	this->sprite->setPosition(pos);
}

/// <summary>
/// Processes this instance.
/// </summary>
void Player::process()
{
	this->hitDetection();
	this->detectEdge();
	this->processPowerUps();

	if (getHealth() <= 0)
	{
		setDeleted(true);
		resourceHandler->getSound(ResourceHandler::Sound::FX_ENEMY_DEATH).play();
	}
}

/// <summary>
/// This function process a activated power
/// </summary>
void Player::processPowerUps()
{
	// Temporary Pulsegun powerup (lasts 6 seconds)
	if (pulsateGun && pwrUpClock.getElapsedTime().asSeconds() < 6)
	{

		// Check if the clock  has elapsed 1 second
		if (pulseClock.getElapsedTime().asMilliseconds() > 1000)
		{

			// Creates a circular bullet pattern.
			for (int i = 1; i < 20; i += 1)
			{

				// Gets a bullet, sets the owner to this, sets the bullet rotation, sets the bullet position, pushes the bullet to the bullet list.
				std::unique_ptr<Bullet> b = getBulletFactory().requestObject(Bullet::Type::standardShot);
				b->setOwner(this->getType());
				b->setRotation(i, sf::Vector2f(150, 150));
				b->sprite->setPosition(this->sprite->getPosition().x, this->sprite->getPosition().y);
				getBullets().push_back(std::move(b));
			}

			pulseClock.restart();
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

	if (sprite->getPosition().x >= window.getView().getSize().x - sprite->getRadius())
	{
		sprite->setPosition(window.getView().getSize().x - sprite->getRadius(), sprite->getPosition().y);
	}

	//## Y AXIS
	if (sprite->getPosition().y - sprite->getRadius() <= 0)
	{
		sprite->setPosition(sprite->getPosition().x, sprite->getRadius());
	}

	if (sprite->getPosition().y >= window.getView().getSize().y - sprite->getRadius())
	{
		sprite->setPosition(sprite->getPosition().x, window.getView().getSize().y - sprite->getRadius());
	}
}


/// <summary>
/// Draws the highscore stats, this function runs in the secondary view.
/// </summary>
/// <param name="highScoreList">The high score list as a reference</param>
void Player::drawStats(std::list<std::shared_ptr<HighScoreItem>>& highScoreList)
{
	// Draw Health
	sf::Text txtHealth;
	txtHealth.setFont(resourceHandler->getFont(ResourceHandler::Fonts::SANSATION));
	std::string dead = ((getHealth() <= 0) ? "Dead" : "");
	txtHealth.setString(sf::String(dead));
	txtHealth.setCharacterSize(25);
	txtHealth.setPosition(0, 0);
	txtHealth.setColor(sf::Color::White);

	// Sets initial heart location
	int heartY = 0;
	int heartX = 5;

	// Iterate each of the "healths", and display hearts
	for (int i = 1; i <= getHealth(); i++)
	{

		// Draw a heart
		std::shared_ptr<sf::Sprite> sprite = std::shared_ptr<sf::Sprite>(new sf::Sprite);
		sprite->setTexture(resourceHandler->getTexture(ResourceHandler::Texture::HEART), true);
		sprite->setPosition(heartX, heartY);
		sprite->setScale(0.05f, 0.05f);
		window.draw(*sprite);

		// Append the width of an haeart to the X position
		heartX += 35;
	}
	window.draw(txtHealth);

	// Draw Score
	sf::Text txtScore;
	txtScore.setFont(resourceHandler->getFont(ResourceHandler::Fonts::SANSATION));
	txtScore.setString(sf::String("Score: " + std::to_string((int)playerScore)));
	txtScore.setCharacterSize(25);
	txtScore.setPosition(heartX + 10 + txtHealth.getGlobalBounds().width, 0);
	txtScore.setColor(sf::Color::White);
	window.draw(txtScore);


	// Draw the stage highscore title
	int initY = 0;
	sf::Text txtHighScoreTitle;
	txtHighScoreTitle.setFont(resourceHandler->getFont(ResourceHandler::Fonts::SANSATION));
	txtHighScoreTitle.setString(sf::String("HS: "));
	txtHighScoreTitle.setCharacterSize(25);
	txtHighScoreTitle.setPosition(txtScore.getPosition().x + txtScore.getGlobalBounds().width + 10, initY);
	txtHighScoreTitle.setColor(sf::Color::White);
	window.draw(txtHighScoreTitle);

	// Draw top 1
	if(!highScoreList.empty())
	{
		std::shared_ptr<HighScoreItem> i = highScoreList.front();
		sf::Text txtHighScore;
		txtHighScore.setFont(resourceHandler->getFont(ResourceHandler::Fonts::SANSATION));
		txtHighScore.setString(sf::String(i->playerName + ": " + std::to_string((int)i->score)));
		txtHighScore.setCharacterSize(25);
		txtHighScore.setPosition(txtHighScoreTitle.getPosition().x + txtHighScoreTitle.getGlobalBounds().width, initY);
		txtHighScore.setColor(sf::Color::White);
		window.draw(txtHighScore);
	}



}

/// <summary>
/// Adds to the score.
/// </summary>
/// <param name="score">The score.</param>
void Player::addScore(float score)
{
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
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && normalShotClock.getElapsedTime().asMilliseconds() > 150){

		// Request a standard bullet and sets the appropriate data
		std::unique_ptr<Bullet> b = getBulletFactory().requestObject(Bullet::Type::standardShot);
		b->setOwner(getType());
		b->sprite->setPosition(sprite->getPosition().x, sprite->getPosition().y - 10);

		// Play shoot sound
		resourceHandler->getSound(ResourceHandler::Sound::FX_STANDARD_SHOT).play();

		// Push the bullet to the bullet list
		getBullets().push_back(std::move(b));

		// Restart the clock
		normalShotClock.restart();
	}

	// Player's right mouse click shoot handler
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && specialShotClock.getElapsedTime().asMilliseconds() > 1000){

		// Request a heavy shot bullet and set the appropriate data
		std::unique_ptr<Bullet> b = getBulletFactory().requestObject(Bullet::Type::heavyShot);
		b->setOwner(getType());
		b->sprite->setPosition(sprite->getPosition().x, sprite->getPosition().y - 10);

		// Play shoot sound
		resourceHandler->getSound(ResourceHandler::Sound::FX_HEAVY_SHOT).play();

		// Push the bullet to the bullet list
		getBullets().push_back(std::move(b));

		// Restart the clock
		specialShotClock.restart();
	}

	if (event.type == sf::Event::MouseMoved)
	{
		// Waiting for update: https://github.com/LaurentGomila/SFML/pull/396
		int current_x = sf::Mouse::getPosition(window).x, current_y = sf::Mouse::getPosition(window).y;
		int elapsed_x = (window.getView().getSize().x / 2) - current_x, elapsed_y = (window.getView().getSize().y / 2) - current_y;

		if (elapsed_x != 0 || elapsed_y != 0)
		{

			//###################################//
			//#####Mouse Movement Handleing######//
			//###################################//
			//## X axis
			if (sprite->getPosition().x > 0 && sprite->getPosition().x < window.getView().getSize().x)
			{
				sprite->move(-elapsed_x, 0);
			}

			// ## Y axis
			if (sprite->getPosition().y > 0 && sprite->getPosition().y < (window.getView().getSize().y + window.getView().getSize().y))
			{
				sprite->move(0, -elapsed_y);
			}

			sf::Mouse::setPosition(sf::Vector2i((window.getView().getSize().x / 2), (window.getView().getSize().y / 2)), window);
		}
	}
}

/// <summary>
/// Gets the player score.
/// </summary>
/// <returns>integer with the player score</returns>
int Player::getPlayerScore()
{
	return playerScore;
}

/// <summary>
/// Function for activating a power up
/// </summary>
/// <param name="powType">Type of the powwer up to activate</param>
void Player::powerUp(Powerup::PowerUpType powType)
{
	// Health Pack Powerup
	if (powType == Powerup::PowerUpType::HEALTH_INCREMENT)
	{
		if(getHealth() > getStartHealth())(getHealth() + 1);
	}

	// Pulsating gun Powerup
	else if (powType == Powerup::PowerUpType::PULSATING_GUN)
	{
		pulsateGun = true;
		pwrUpClock.restart();
		pulseClock.restart();
	}

}