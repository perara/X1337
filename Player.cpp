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
			   std::unique_ptr<ResourceHandler>& resourceHandler,
			   const sf::Time& timeStep
			   ):playerScore(0),

			   Shooter(window, bFactory, bullets, resourceHandler, timeStep)
{
	this->setType(Shooter::ShooterType::PLAYER);
	sprite = std::shared_ptr<GameShape>(new GameShape(GameShape::CIRCLE, 10));
	this->sprite->setPosition(pos);
}

void Player::process()
{
	this->shooterProcess();
	this->detectEdge();

	if(health<=0)
	{
		deleted=true;
		resourceHandler->getSound(ResourceHandler::Sound::ENEMY_DEATH).play();
	}
}

void Player::detectEdge()
{
	/************************************************************************/
	/* Game Bounds Collision test                                           */
	/************************************************************************/

	// X
	if(sprite->getPosition().x - sprite->getRadius() <= 0)
	{
		sprite->setPosition(sprite->getRadius(), sprite->getPosition().y);
	}

	if(sprite->getPosition().x >= window.getView().getSize().x - sprite->getRadius())
	{
		sprite->setPosition(window.getView().getSize().x - sprite->getRadius(), sprite->getPosition().y);
	}

	// Y
	if(sprite->getPosition().y - sprite->getRadius() <= 0)
	{
		sprite->setPosition(sprite->getPosition().x, sprite->getRadius());
	}


	if(sprite->getPosition().y >= window.getView().getSize().y - sprite->getRadius())
	{
		sprite->setPosition(sprite->getPosition().x, window.getView().getSize().y - sprite->getRadius());
	}
}


void Player::drawStats(std::list<std::shared_ptr<HighScoreItem>>& highScoreList)
{
	// Draw Health
	sf::Text txtHealth;
	txtHealth.setFont(resourceHandler->getFont(ResourceHandler::Fonts::SANSATION));
	std::string dead = ((health < 0) ? "Dead" : "");
	txtHealth.setString(sf::String("Health: " + dead));
	txtHealth.setCharacterSize(25);
	txtHealth.setPosition(20,20);
	txtHealth.setColor(sf::Color::White);

	int heartX = 105;
	for(int i = 0; i < getHealth(); i++)
	{

		std::shared_ptr<sf::Sprite> sprite = std::shared_ptr<sf::Sprite>(new sf::Sprite);
		sprite->setTexture(resourceHandler->getTexture(ResourceHandler::Texture::HEART), true);
		sprite->setPosition(heartX, 20);
		sprite->setScale(0.05f,0.05f);
		window.draw(*sprite);
		heartX = heartX + 35;
	}

	// Draw Score
	sf::Text txtScore;
	txtScore.setFont(resourceHandler->getFont(ResourceHandler::Fonts::SANSATION));
	std::stringstream scrStr;
	scrStr << playerScore;//add number to the stream
	txtScore.setString(sf::String("Score: " + scrStr.str()));
	txtScore.setCharacterSize(25);
	txtScore.setPosition(20,80);
	txtScore.setColor(sf::Color::White);


	window.draw(txtHealth);
	window.draw(txtScore);


	// Title
	int initY = 150;
	sf::Text txtHighScoreTitle;
	txtHighScoreTitle.setFont(resourceHandler->getFont(ResourceHandler::Fonts::SANSATION));
	txtHighScoreTitle.setString(sf::String("Stage highscore: "));
	txtHighScoreTitle.setCharacterSize(25);
	txtHighScoreTitle.setPosition(20,initY);
	txtHighScoreTitle.setColor(sf::Color::White);
	window.draw(txtHighScoreTitle);

	// Draw Highscore for current scene
	int num = 1;
	for(auto& i : highScoreList)
	{
		initY += 30;
		std::stringstream theScore;
		theScore << i->score;

		std::stringstream rank;
		rank << num;

		sf::Text txtHighScore;
		txtHighScore.setFont(resourceHandler->getFont(ResourceHandler::Fonts::SANSATION));
		txtHighScore.setString(sf::String(rank.str() + "\t" + i->playerName + "\t" + theScore.str()));
		txtHighScore.setCharacterSize(25);
		txtHighScore.setPosition(20,initY);
		txtHighScore.setColor(sf::Color::White);
		window.draw(txtHighScore);

		num++;
	}


}

void Player::addScore(float score)
{
	playerScore+=score;
}

void Player::setHealth(int value)
{
	health = value;
}

void Player::input(sf::Event& event)
{
	/* Shoot handler */
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && normalShotClock.getElapsedTime().asMilliseconds() > 150){
		std::unique_ptr<Bullet> b = getBulletFactory().requestObject(Bullet::Type::standardShot);
		b->setOwner(getType());
		b->setPosition(sprite->getPosition().x , sprite->getPosition().y - 10);
		resourceHandler->getSound(ResourceHandler::Sound::STANDARD_SHOT).play();

		getBullets().push_back(std::move(b));

		if(getHealth() > 0) scoreTime += 1;
		normalShotClock.restart();
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && specialShotClock.getElapsedTime().asMilliseconds() > 1000){
		std::unique_ptr<Bullet> b = getBulletFactory().requestObject(Bullet::Type::heavyShot);
		b->setOwner(getType());
		b->setPosition(sprite->getPosition().x , sprite->getPosition().y - 10);
		resourceHandler->getSound(ResourceHandler::Sound::HEAVY_SHOT).play();
		getBullets().push_back(std::move(b));

		if(getHealth() > 0) scoreTime += 1;
		specialShotClock.restart();
	}


	if(event.type == sf::Event::MouseMoved)
	{
		int current_x = sf::Mouse::getPosition(window).x, current_y = sf::Mouse::getPosition(window).y;
		int elapsed_x = (window.getView().getSize().x / 2) - current_x, elapsed_y = (window.getView().getSize().y / 2) - current_y;

		if(elapsed_x != 0 || elapsed_y != 0)
		{

			/************************************************************************/
			/* Mouse Movement Handling                                              */
			/************************************************************************/
			if(sprite->getPosition().x > 0 && sprite->getPosition().x < window.getView().getSize().x)
			{
				sprite->move(-elapsed_x  , 0);
			}

			if(sprite->getPosition().y > 0 && sprite->getPosition().y < (window.getView().getSize().y + window.getView().getSize().y))
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