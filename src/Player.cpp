#include "Player.h"
#include "Bullet.h"
#include "BulletFactory.h"
#include "GameShape.h"
#include "Log.h"
#include "ResourceHandler.h"
#include <sstream>

sf::Clock normalShotClock;
sf::Clock specialShotClock;
float scoreTime;

/// <summary>
/// Initializes a new instance of the <see cref="Player"/> class.
/// </summary>
/// <param name="window">The render window.</param>
/// <param name="pos">The initial startposition of the player</param>
/// <param name="radius">The radius.</param>
/// <param name="bFactory">The <see cref=BulletFactory"></param>
/// <param name="sceneBulletListCallback">The scene object call back. This is basicly a function pointer to the corresponding world function "addObject" Reason for passing this is so we can add bullets to the Scene loop</param>
Player::Player(sf::RenderWindow& window,
	sf::Vector2f pos,
	int radius, BulletFactory& bFactory,
	std::list<std::unique_ptr<Bullet>>& bullets,
	std::shared_ptr<ResourceHandler>& resourceHandler,
	const sf::Time& timeStep,
	const bool hardMode
	) :
	playerScore(0),
	pulsateGun(false),

	Shooter(window, bFactory, bullets, resourceHandler, timeStep)
{
	if (!hardMode)
	{
		setHealth(5);
	}
	else
	{
		setHealth(1);
	}

	this->setType(Shooter::ShooterType::PLAYER);
	sprite = std::shared_ptr<GameShape>(new GameShape(GameShape::ShapeType::PLAYER_SHIP));
	sprite->setTexture(&resourceHandler->getTexture(ResourceHandler::Texture::PLAYER_SHIP));
	sprite->setOutlineThickness(1);
	sprite->setOutlineColor(sf::Color::Cyan);
	this->sprite->setPosition(pos);
}

void Player::process()
{
	this->hitDetection();
	this->detectEdge();
	this->processPowerUps();

	if (getHealth() <= 0)
	{
		deleted = true;
		resourceHandler->getSound(ResourceHandler::Sound::ENEMY_DEATH).play();
	}
}

// This function process a activated power
void Player::processPowerUps()
{
	// Temporary Pulsegun
	if (pulsateGun && pwrUpClock.getElapsedTime().asSeconds() < 6)
	{

		if (pulseClock.getElapsedTime().asMilliseconds() > 1000)
		{
			for (int i = 1; i < 20; i += 1)
			{
				std::unique_ptr<Bullet> b = getBulletFactory().requestObject(Bullet::Type::standardShot);
				b->setOwner(this->getType());
				b->setRotation(i, sf::Vector2f(150, 150));
				b->sprite->setPosition(this->sprite->getPosition().x, this->sprite->getPosition().y);
				getBullets().push_back(std::move(b));
			}
			pulseClock.restart();
		}
	}
	else
	{
		pulsateGun = false;
	}


}

void Player::detectEdge()
{
	/************************************************************************/
	/* Game Bounds Collision test                                           */
	/************************************************************************/

	// X
	if (sprite->getPosition().x - sprite->getRadius() <= 0)
	{
		sprite->setPosition(sprite->getRadius(), sprite->getPosition().y);
	}

	if (sprite->getPosition().x >= window.getView().getSize().x - sprite->getRadius())
	{
		sprite->setPosition(window.getView().getSize().x - sprite->getRadius(), sprite->getPosition().y);
	}

	// Y
	if (sprite->getPosition().y - sprite->getRadius() <= 0)
	{
		sprite->setPosition(sprite->getPosition().x, sprite->getRadius());
	}


	if (sprite->getPosition().y >= window.getView().getSize().y - sprite->getRadius())
	{
		sprite->setPosition(sprite->getPosition().x, window.getView().getSize().y - sprite->getRadius());
	}
}


void Player::drawStats(std::list<std::shared_ptr<HighScoreItem>>& highScoreList)
{
	// Draw Health
	sf::Text txtHealth;
	txtHealth.setFont(resourceHandler->getFont(ResourceHandler::Fonts::SANSATION));
	std::string dead = ((getHealth() <= 0) ? "Dead" : "");
	txtHealth.setString(sf::String("Health: " + dead));
	txtHealth.setCharacterSize(25);
	txtHealth.setPosition(20, 20);
	txtHealth.setColor(sf::Color::White);

	int heartY = 20;
	int heartX = 105;
	for (int i = 1; i <= getHealth(); i++)
	{

		std::shared_ptr<sf::Sprite> sprite = std::shared_ptr<sf::Sprite>(new sf::Sprite);
		sprite->setTexture(resourceHandler->getTexture(ResourceHandler::Texture::HEART), true);
		sprite->setPosition(heartX, heartY);
		sprite->setScale(0.05f, 0.05f);
		window.draw(*sprite);
		heartX += 35;

		if (i % 5 == 0)
		{
			heartY += sprite->getGlobalBounds().height;
			heartX = 105;
		}

	}

	// Draw Score
	sf::Text txtScore;
	txtScore.setFont(resourceHandler->getFont(ResourceHandler::Fonts::SANSATION));
	txtScore.setString(sf::String("Score: " + std::to_string((int)playerScore)));
	txtScore.setCharacterSize(25);
	txtScore.setPosition(20, 80);
	txtScore.setColor(sf::Color::White);


	window.draw(txtHealth);
	window.draw(txtScore);


	// Title
	int initY = 150;
	sf::Text txtHighScoreTitle;
	txtHighScoreTitle.setFont(resourceHandler->getFont(ResourceHandler::Fonts::SANSATION));
	txtHighScoreTitle.setString(sf::String("Stage highscore: "));
	txtHighScoreTitle.setCharacterSize(25);
	txtHighScoreTitle.setPosition(20, initY);
	txtHighScoreTitle.setColor(sf::Color::White);
	window.draw(txtHighScoreTitle);

	// Draw Highscore for current scene
	int rank = 1;
	for (auto& i : highScoreList)
	{
		if (rank == 10) break;
		initY += 30;

		sf::Text txtHighScore;
		txtHighScore.setFont(resourceHandler->getFont(ResourceHandler::Fonts::SANSATION));
		txtHighScore.setString(sf::String(std::to_string((int)rank) + "\t" + i->playerName + "\t" + std::to_string((int)i->score)));
		txtHighScore.setCharacterSize(25);
		txtHighScore.setPosition(20, initY);
		txtHighScore.setColor(sf::Color::White);
		window.draw(txtHighScore);

		rank++;
	}


}

void Player::addScore(float score)
{
	playerScore += score;
}

void Player::input(sf::Event& event)
{
	/* Shoot handler */
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && normalShotClock.getElapsedTime().asMilliseconds() > 150){
		std::unique_ptr<Bullet> b = getBulletFactory().requestObject(Bullet::Type::standardShot);
		b->setOwner(getType());
		b->sprite->setPosition(sprite->getPosition().x, sprite->getPosition().y - 10);
		resourceHandler->getSound(ResourceHandler::Sound::STANDARD_SHOT).play();

		getBullets().push_back(std::move(b));

		if (getHealth() > 0) scoreTime += 1;
		normalShotClock.restart();
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && specialShotClock.getElapsedTime().asMilliseconds() > 1000){
		std::unique_ptr<Bullet> b = getBulletFactory().requestObject(Bullet::Type::heavyShot);
		b->setOwner(getType());
		b->sprite->setPosition(sprite->getPosition().x, sprite->getPosition().y - 10);
		resourceHandler->getSound(ResourceHandler::Sound::HEAVY_SHOT).play();
		getBullets().push_back(std::move(b));

		if (getHealth() > 0) scoreTime += 1;
		specialShotClock.restart();
	}


	if (event.type == sf::Event::MouseMoved)
	{
		int current_x = sf::Mouse::getPosition(window).x, current_y = sf::Mouse::getPosition(window).y;
		int elapsed_x = (window.getView().getSize().x / 2) - current_x, elapsed_y = (window.getView().getSize().y / 2) - current_y;

		if (elapsed_x != 0 || elapsed_y != 0)
		{

			/************************************************************************/
			/* Mouse Movement Handling                                              */
			/************************************************************************/
			if (sprite->getPosition().x > 0 && sprite->getPosition().x < window.getView().getSize().x)
			{
				sprite->move(-elapsed_x, 0);
			}

			if (sprite->getPosition().y > 0 && sprite->getPosition().y < (window.getView().getSize().y + window.getView().getSize().y))
			{
				sprite->move(0, -elapsed_y);
			}
			sf::Mouse::setPosition(sf::Vector2i((window.getView().getSize().x / 2), (window.getView().getSize().y / 2)), window);
		}
	}

}

int Player::getPlayerScore()
{
	return playerScore;
}

void Player::powerUp(Powerup::PowerUpType powType)
{
	if (powType == Powerup::PowerUpType::HEALTH_INCREMENT)
	{
		setHealth(getHealth() + 1);
	}
	if (powType == Powerup::PowerUpType::PULSATING_GUN)
	{
		pulsateGun = true;
		pwrUpClock.restart();
		pulseClock.restart();
	}

}