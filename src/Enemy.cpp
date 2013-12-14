#include "Enemy.h"
#include "GameShape.h"
#include "BulletFactory.h"
#include "Bullet.h"
#include "Log.h"
/// <summary>
/// Initializes a new instance of the <see cref="Enemy"/> class.
/// </summary>
/// <param name="window">The window.</param>
/// <param name="path">The path.</param>
/// <param name="emoteQueue">The emote queue specifies the queue of emotes loaded from script.</param>
/// <param name="type">The type.</param>
/// <param name="repeat">The repeat.</param>
/// <param name="bFactory">The b factory.</param>
/// <param name="bullets">The bullets.</param>
/// <param name="resourceHandler">The resource handler.</param>
/// <param name="timeStep">The time step.</param>
Enemy::Enemy(sf::RenderWindow& window,
			 std::queue<sf::Vector3f> path,
			 std::list<std::pair<int, std::string>> emoteQueue,
			 int type, int repeat, BulletFactory& bFactory,
			 std::list<std::unique_ptr<Bullet>>& bullets,
			 std::shared_ptr<ResourceHandler>& resourceHandler,
			 const sf::Time& timeStep
			 ) :
pathTemplate(path), // Const, not to be changed (The template is copyed when a path is over in repeat mode)
	emoteQueue(emoteQueue),
	repeat(repeat),
	secondRot(0),
	Shooter(window, bFactory, bullets, resourceHandler, timeStep)
{
	// Start the enemy clock
	this->enemyClock.restart();

	// Sets the type of the Shooter to enemy
	this->shooterType = Shooter::ShooterType::ENEMY;

	// Check which type the enemy is, from this we determine, health, total health , score value and textures,
	if (type == 1) // REGULAR
	{
		setHealth(2);
		setStartHealth(2);
		sprite = std::unique_ptr<GameShape>(new GameShape(GameShape::ShapeType::STARSHIP));
		sprite->setTexture(&resourceHandler->getTexture(ResourceHandler::Texture::ENEMY_SHIP));
		this->setEnemyType(Enemy::EnemyType::REGULAR);
		this->setScoreValue(100);
	}
	else if (type == 2) // Chubby Mob
	{
		setHealth(10);
		setStartHealth(10);
		sprite = std::unique_ptr<GameShape>(new GameShape(GameShape::ShapeType::CIRCLE, 40, 30));
		sprite->setTexture(&resourceHandler->getTexture(ResourceHandler::Texture::CHUBBY_SHIP_TEXTURE));
		this->setEnemyType(Enemy::EnemyType::CHUBBY);
		this->setScoreValue(250);

	}
	else if (type == 3) // BOSS
	{
		setHealth(250);
		setStartHealth(250);
		sprite = std::unique_ptr<GameShape>(new GameShape(GameShape::ShapeType::BOSS));
		sprite->setTexture(&resourceHandler->getTexture(ResourceHandler::Texture::BOSS));
		this->setEnemyType(Enemy::EnemyType::BOSS);
		this->setScoreValue(500);

	}
	else if (type == 4) // Umbasta boss
	{
		setHealth(750);
		setStartHealth(750);
		sprite = std::unique_ptr<GameShape>(new GameShape(GameShape::ShapeType::CIRCLE, 80, 30));
		sprite->setTexture(&resourceHandler->getTexture(ResourceHandler::Texture::BOSS_DEATHSTAR_TEXTURE));
		this->setEnemyType(Enemy::EnemyType::DEATHSTAR);
		this->setScoreValue(12000);
	}

#ifdef _DEBUG
	sprite->setOutlineThickness(1);
	sprite->setOutlineColor(sf::Color::Cyan);
#endif

	setInitPath();
}

/// <summary>
/// Resets the path with use of the pathTemplate. And then sets current path to the first value in the queue.
/// </summary>
void Enemy::setInitPath()
{

	setDeleted(false);
	this->path = pathTemplate;
	this->currentPath = this->path.front();
	this->path.pop();

	this->sprite->setPosition(currentPath.x, currentPath.y);
}

/// <summary>
/// Finalizes an instance of the <see cref="Enemy"/> class.
/// </summary>
Enemy::~Enemy(){
	LOGD("Deconstructor called for: Enemy#" << this);
}


/// <summary>
/// Draws the enemy
/// </summary>
void Enemy::draw()
{
	// Draw Enemy
	this->window.draw(*this->sprite);


	// Calculates the health bar percentage
	float percent = (100.0f / getStartHealth()) * getHealth();
	float barWidth = (this->sprite->getGlobalBounds().width) / 100.0f;

	// Draws the health bar
	sf::RectangleShape healthBar;
	healthBar.setSize(sf::Vector2f((barWidth * percent) / 2, 2));
	healthBar.setPosition(
		this->sprite->getGlobalBounds().left + (barWidth * 25),
		this->sprite->getGlobalBounds().top - 5);
	healthBar.setFillColor(sf::Color::Green);
	window.draw(healthBar);

	/*sf::FloatRect bounds = this->sprite->getGlobalBounds();
	sf::RectangleShape af(sf::Vector2f(bounds.width,bounds.height));
	af.setPosition(bounds.left, bounds.top);
	af.setOutlineColor(sf::Color(141,23,22,23));
	af.setFillColor(sf::Color(255,255,255,150));
	this->window.draw(af);*/

}


/// <summary>
/// Enemy shoot handler
/// </summary>
/// <param name="shoot">Which type of shot it is.</param>
void Enemy::shoot(int shoot)
{
	// SHOOT -1 = undefined (no shooting)
	// SHOOT 0 = undefined (no shooting)
	// SHOOT 3  = SPECIAL ATTACK
	// SHOOT 1 = Normal attack which differ depending on the enemy

	// Check if shooting is of.
	if (shoot != -1 && shoot != 0)
	{

		// Check if its a regular enemy
		if (getEnemyType() == Enemy::EnemyType::REGULAR)
		{
			// Gets a bullet and set the appropriate data, then push to the bulletlist
			std::unique_ptr<Bullet> b = getBulletFactory().requestObject(Bullet::Type::standardShot);
			b->setOwner(this->getType());
			b->sprite->setPosition(this->sprite->getPosition().x, this->sprite->getPosition().y - 10);
			getBullets().push_back(std::move(b));

		}

		// Check if its a Chubby enemy
		else if (getEnemyType() == Enemy::EnemyType::CHUBBY)
		{
			// Retrieve 3 bullets and defines a start position for the bullet.
			std::list<std::unique_ptr<Bullet>> bat = getBulletFactory().requestBatch(3, Bullet::Type::standardShot);
			int startX = (this->sprite->getPosition().x) - (this->sprite->getGlobalBounds().width / 2) + 10;

			// Iterate through the bullet request , set properties and push back to bullet list
			for (auto& i : bat)
			{
				std::unique_ptr<Bullet> bs = std::move(i);
				bs->setOwner(this->getType());
				bs->sprite->setPosition(startX, this->sprite->getPosition().y - 10);
				getBullets().push_back(std::move(bs));
				startX += this->sprite->getGlobalBounds().width / 4;
			}

		}

		// Check if its a BOSS enemy
		else if (getEnemyType() == Enemy::EnemyType::BOSS)
		{

			// Gets a bullet and set the appropriate data, then push to the bulletlist
			std::list<std::unique_ptr<Bullet>> bat = getBulletFactory().requestBatch(10, Bullet::Type::standardShot);
			int startX = (this->sprite->getPosition().x) - (this->sprite->getGlobalBounds().width / 2);

			// Iterate through the bullet request , set properties and push back to bullet list
			for (auto& i : bat)
			{
				std::unique_ptr<Bullet> bs = std::move(i);
				bs->setOwner(this->getType());
				bs->sprite->setPosition(startX, this->sprite->getPosition().y - 10);
				getBullets().push_back(std::move(bs));
				startX += this->sprite->getGlobalBounds().width / 10;
			}

		}

		// Check if its a DEATHSTAR enemy
		else if (getEnemyType() == Enemy::EnemyType::DEATHSTAR)
		{

			if (shoot == 3) // Special attack
			{
				std::unique_ptr<Bullet> b = getBulletFactory().requestObject(Bullet::Type::standardShot);
				b->setOwner(this->getType());
				b->setSpeed(sf::Vector2f(0.0f, 350.0f));
				b->sprite->setFillColor(sf::Color::Green);
				b->sprite->setPosition(this->sprite->getPosition().x, this->sprite->getPosition().y - 10);
				getBullets().push_back(std::move(b));
			}

			// Circular shoot pattern
			else
			{
				for (float i = 0; i < 360; i += 2)
				{
					std::unique_ptr<Bullet> b = getBulletFactory().requestObject(Bullet::Type::standardShot);
					b->setOwner(this->getType());
					b->setRotation(i + secondRot, sf::Vector2f(150, 150));
					b->sprite->setPosition(this->sprite->getPosition().x, this->sprite->getPosition().y);
					getBullets().push_back(std::move(b));

				}

				// randomize 2nd degree rotation
				secondRot++;
			}
		}
	}
}

void Enemy::emotes()
{
	if(!emoteQueue.empty())
	{
		if(emoteQueue.front().first > (int)((100.0f / getStartHealth()) * getHealth()))
		{
			std::string soundToPlay = emoteQueue.front().second;
			emoteQueue.pop_front();
			resourceHandler->getSoundByEmoteName(soundToPlay).play();
		}
	}
}

void Enemy::movement()
{
	// Start
	sf::Vector2f length;
	length.x = abs(this->currentPath.x - path.front().x);
	length.y = abs(this->currentPath.y - path.front().y);

	// End
	sf::Vector2f currentPosition;
	currentPosition.x = abs(this->currentPath.x - this->sprite->getPosition().x);
	currentPosition.y = abs(this->currentPath.y - this->sprite->getPosition().y);


	// The difference between the goto and the current position
	float dx = this->path.front().x - this->currentPath.x;
	float dy = this->path.front().y - this->currentPath.y;
	float len = sqrtf(dx * dx + dy * dy);

	// Dfeines the speed of the enemy
	dx = (dx / len) * timeStep.asSeconds() * 50;
	dy = (dy / len) * timeStep.asSeconds() * 50;

	// Normal Shoot processing
	if (this->enemyClock.getElapsedTime().asMilliseconds() > 400)
	{
		this->shoot(this->currentPath.z);
		this->enemyClock.restart();
	}

	// Special Cases shoot processing (Like deathstar lazer)
	if (this->currentPath.z == 3) // check if its a special case (3 is special attack for deathstar)
	{
		this->shoot(this->currentPath.z);
	}

	// Check if the enemy has reached a path point or not ( move him if not)
	if (
		currentPosition.x < length.x ||
		currentPosition.y < length.y
		)
	{

		if (currentPosition.x < length.x)
		{
			this->sprite->move(dx, 0);
		}

		if (currentPosition.y < length.y)
		{
			this->sprite->move(0, dy);
		}

	}

	// ENemy has travelled the path, get a new one
	else
	{

		// Check if there is any other paths to go
		if (path.size() > 1)
		{
			currentPath = path.front();
			path.pop();
		}

		// If there is no paths to go.
		else
		{
			LOGD("Enemy#" << this << " delete flag set");
			setDeleted(true);

			// If the enemy has repeat option on, reinit it.
			if (this->getRepeat() == 1){
				setInitPath();
			}

		}

	}

}

/// <summary>
/// Processes this instance.
/// </summary>
void Enemy::process()
{
	this->hitDetection();
	this->emotes();
	this->movement();
}


/// <summary>
/// Gets the repeat.
/// </summary>
/// <returns>Repeat flag</returns>
int Enemy::getRepeat()
{
	return this->repeat;
}

/// <summary>
/// Sets the type of the enemy.
/// </summary>
/// <param name="type">The enemy type.</param>
void Enemy::setEnemyType(Enemy::EnemyType type)
{
	this->enemyType = type;
}

/// <summary>
/// Gets the type of the enemy.
/// </summary>
/// <returns>Returns the enemy type</returns>
Enemy::EnemyType Enemy::getEnemyType()
{
	return this->enemyType;
}
