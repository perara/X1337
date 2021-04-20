#include "../include/Enemy.h"
#include "../include/GameShape.h"
#include "../include/BulletFactory.h"
#include <cmath>
#include <memory>
#include <utility>
#include <spdlog/spdlog.h>

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
Enemy::Enemy(Renderer& window,
	std::queue<VectorN> path,
	std::list<std::pair<int, std::string>> emoteQueue,
	int type, int repeat, int delay, BulletFactory& bFactory,
	std::list<std::unique_ptr<Bullet>>& bullets,
	std::shared_ptr<ResourceManager>& resourceHandler,
	std::list<std::shared_ptr<Shooter>>& objects,
	const sf::Time& timeStep
	) :
	Shooter(window, bFactory, bullets, resourceHandler, objects, timeStep),

    repeat(repeat),
    spawnDelay(delay),
    acceleration(0),
    speed(sf::Vector2f(50, 50)),
    pathTemplate(std::move(path)), // Const, not to be changed (The template is copyed when a path is over in repeat mode)
    emoteQueue(std::move(emoteQueue)),
    enemyType(Constants::EnemyC::Type::NONE),
    secondRot(0)

{
	// Sets the type of the Shooter to enemy
	this->shooterType = Constants::ShooterType::ENEMY;

	// Check which type the enemy is, from this we determine, health, total health , score value and textures,
	if (type == 1) // REGULAR
	{
		setHealth(2);
		setStartHealth(2);
		sprite = std::make_unique<GameShape>(Constants::GameShapeC::Type::STARSHIP);
		sprite->setTexture(&resourceHandler->getTexture(Constants::ResourceC::Texture::ENEMY_SHIP));
		this->setEnemyType(Constants::EnemyC::Type::REGULAR);
		this->setScoreValue(100);
	}
	else if (type == 2) // Chubby Mob
	{
		setHealth(10);
		setStartHealth(10);
		sprite = std::make_unique<GameShape>(Constants::GameShapeC::Type::CIRCLE, 40, 30);
		sprite->setTexture(&resourceHandler->getTexture(Constants::ResourceC::Texture::CHUBBY_SHIP_TEXTURE));
		this->setEnemyType(Constants::EnemyC::Type::CHUBBY);
		this->setScoreValue(250);

	}
	else if (type == 3) // BOSS
	{
		setHealth(250);
		setStartHealth(250);
		sprite = std::make_unique<GameShape>(Constants::GameShapeC::Type::BOSS);
		sprite->setTexture(&resourceHandler->getTexture(Constants::ResourceC::Texture::BOSS));
		this->setEnemyType(Constants::EnemyC::Type::BOSS);
		this->setScoreValue(500);

	}
	else if (type == 4) // Umbasta boss
	{
		setHealth(750);
		setStartHealth(750);
		sprite = std::make_unique<GameShape>(Constants::GameShapeC::Type::CIRCLE, 80, 30);
		sprite->setTexture(&resourceHandler->getTexture(Constants::ResourceC::Texture::BOSS_DEATHSTAR_TEXTURE));
		this->setEnemyType(Constants::EnemyC::Type::DEATHSTAR);
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
	this->acceleration = 0;
	this->path.pop();

	this->sprite->setPosition(currentPath.x, currentPath.y);
}

/// <summary>
/// Finalizes an instance of the <see cref="Enemy"/> class.
/// </summary>
Enemy::~Enemy(){
    SPDLOG_INFO("Deconstructor called for: Enemy#" , (void*)this);
}


/// <summary>
/// Draws the enemy
/// </summary>
void Enemy::draw()
{
	// Draw Enemy
	this->renderer.draw(*this->sprite);


	// Calculates the health bar percentage
    float percent = (100.0f / (float)getStartHealth()) * (float)getHealth();
    float barWidth = (this->sprite->getGlobalBounds().width) / 100.0f;

	// Draws the health bar
	sf::RectangleShape healthBar;
	healthBar.setSize(sf::Vector2f((barWidth * percent) / 2, 2));
	healthBar.setPosition(
		this->sprite->getGlobalBounds().left + (barWidth * 25),
		this->sprite->getGlobalBounds().top - 5);
	healthBar.setFillColor(sf::Color::Green);
	renderer.draw(healthBar);

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
		if (getEnemyType() == Constants::EnemyC::Type::REGULAR)
		{
			// Gets a bullet and set the appropriate data, then push to the bulletlist
			std::unique_ptr<Bullet> b = getBulletFactory().requestObject(Constants::BulletType::standardShot);
			b->setOwner(this);
			b->sprite->setPosition(this->sprite->getPosition().x, this->sprite->getPosition().y - 10);
			getBullets().push_back(std::move(b));

		}

		// Check if its a Chubby enemy
		else if (getEnemyType() == Constants::EnemyC::Type::CHUBBY)
		{
			// Retrieve 3 bullets and defines a start position for the bullet.
			std::list<std::unique_ptr<Bullet>> bat = getBulletFactory().requestBatch(3, Constants::BulletType::standardShot);
			auto startX = sprite->getPosition().x - (this->sprite->getGlobalBounds().width / 2) + 10;

			// Iterate through the bullet request , set properties and push back to bullet list
			for (auto& i : bat)
			{
				std::unique_ptr<Bullet> bs = std::move(i);
				bs->setOwner(this);
				bs->sprite->setPosition(startX, this->sprite->getPosition().y - 10);
				getBullets().push_back(std::move(bs));
				startX += this->sprite->getGlobalBounds().width / 4;
			}

		}

		// Check if its a BOSS enemy
		else if (getEnemyType() == Constants::EnemyC::Type::BOSS)
		{

			// Gets a bullet and set the appropriate data, then push to the bulletlist
			std::list<std::unique_ptr<Bullet>> bat = getBulletFactory().requestBatch(10, Constants::BulletType::standardShot);
			float startX = (this->sprite->getPosition().x) - (this->sprite->getGlobalBounds().width / 2);

			// Iterate through the bullet request , set properties and push back to bullet list
			for (auto& i : bat)
			{
				std::unique_ptr<Bullet> bs = std::move(i);
				bs->setOwner(this);
				bs->sprite->setPosition(startX, this->sprite->getPosition().y - 10);
				getBullets().push_back(std::move(bs));
				startX += this->sprite->getGlobalBounds().width / 10;
			}

		}

		// Check if its a DEATHSTAR enemy
		else if (getEnemyType() == Constants::EnemyC::Type::DEATHSTAR)
		{

			if (shoot == 3 || shoot == 5) // Special attack
			{
				std::unique_ptr<Bullet> b = getBulletFactory().requestObject(Constants::BulletType::standardShot);
				b->setOwner(this);

				// Rotational Lazor (Lol)
				if (shoot == 5)
				{
					secondRot += 0.004f * (float)timeStep.asMilliseconds();
					b->setRotation(secondRot, sf::Vector2f(350.f, 350.0f));
					b->sprite->setFillColor(sf::Color::Red);
					b->sprite->setPosition(this->sprite->getPosition().x, this->sprite->getPosition().y);
				}
				else
				{
					b->setSpeed(sf::Vector2f(0.0f, 350.0f));
					b->sprite->setFillColor(sf::Color::Green);
					b->sprite->setPosition(this->sprite->getPosition().x, this->sprite->getPosition().y + (float)this->sprite->getRadius());
				}
				getBullets().push_back(std::move(b));


			}

			// Circular shoot pattern
			if (shoot == 4)
			{
				for (int i = 0; i < 360; i += 2)
				{
					std::unique_ptr<Bullet> b = getBulletFactory().requestObject(Constants::BulletType::standardShot);
					b->setOwner(this);
					b->setRotation((float)i + secondRot, sf::Vector2f(150, 150));
					b->sprite->setPosition(this->sprite->getPosition().x, this->sprite->getPosition().y);
					getBullets().push_back(std::move(b));

				}

				// randomize 2nd degree rotation
				secondRot++;
			}
		}
	}
}

/// <summary>
/// Defines emotes processing for the enemy
/// </summary>
void Enemy::emotes()
{
	// Emote queue's emotes
	if (!emoteQueue.empty())
	{
		if (emoteQueue.front().first > (int)((100.0f / (float)getStartHealth()) * (float)getHealth()))
		{
			std::string soundToPlay = emoteQueue.front().second;
			emoteQueue.pop_front();
			resourceHandler->getSoundByEmoteName(soundToPlay).play();
		}
	}

	// Pathqueue's emotes
	if (this->currentPath.emote != nullptr)
	{
		this->currentPath.emote->play();
		this->currentPath.emote = nullptr;
	}

}

/// <summary>
/// Defines movement processing for the enemy
/// </summary>
void Enemy::movement()
{
	// do not process movement if sleep is in action
	if (sleepTime < this->currentPath.sleepTime) return;

	// Start point of the path
	sf::Vector2f length;
	length.x = std::abs(this->currentPath.x - path.front().x);
	length.y = std::abs(this->currentPath.y - path.front().y);

	// End point of the path
	sf::Vector2f currentPosition;
	currentPosition.x = std::abs(this->currentPath.x - this->sprite->getPosition().x);
	currentPosition.y = std::abs(this->currentPath.y - this->sprite->getPosition().y);


	// The difference between the goto and current position
	float dx = this->path.front().x - this->currentPath.x;
	float dy = this->path.front().y - this->currentPath.y;
	float len = sqrtf(dx * dx + dy * dy);

	// Defines acceleration
	if (this->currentPath.acceleration != 0)
	{
		// Accelerate
		float accelerateTick = this->currentPath.acceleration * 0.18f;
		if (this->acceleration + accelerateTick > 0){
            this->acceleration += this->currentPath.acceleration * 0.18f;
		}
		else this->acceleration = 0;
	}
	else
	{
		// Deaccelerate
		if (this->acceleration >= 1)
		{
			this->acceleration -= this->currentPath.acceleration * 0.18f;
		}
	}


	dx = (dx / len) * timeStep.asSeconds() * (speed.x + acceleration);
	dy = (dy / len) * timeStep.asSeconds() * (speed.y + acceleration);

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

			// Reset the sleep time
			sleepTime = sf::milliseconds(0);
		}

		// If there is no paths to go.
		else
		{
            SPDLOG_INFO("Enemy#{} delete flag set", (void*)this);
			setDeleted(true);

			// If the enemy has repeat option on, reinit it.
			if (this->getRepeat() == 1){
				setInitPath();
			}

		}

	}

}

/// <summary>
/// Shoot processing. This function is basicly the processing of when the enemy should shoot
/// </summary>
void Enemy::shootProcess()
{
	// Normal Shoot processing (Regular Mobs and shooters with shoot == 1)
	if (this->currentPath.shoot == 1 && shootTime.asMilliseconds() > 400) // Normal shooting
	{
		this->shoot(this->currentPath.shoot);
		shootTime = sf::milliseconds(0);
	}

	// The green Lazor (DEATHSTAR) and/or red rotational beam
	if (this->currentPath.shoot == 3 || this->currentPath.shoot == 5) // check if its a special case (3 is special attack for deathstar)
	{
		this->shoot(this->currentPath.shoot);
	}

	// Circular Shooting Pattern (DEATHSTAR)
	if (this->currentPath.shoot == 4 && shootTime.asMilliseconds() > 400)
	{
		this->shoot(this->currentPath.shoot);
		shootTime = sf::milliseconds(0);
	}
}


/// <summary>
/// Processes this instance.
/// </summary>
void Enemy::process()
{
	// Update sf::Time's in enemy
	shootTime += timeStep;
	sleepTime += timeStep;

	this->hitDetection();
	this->emotes();
	this->movement();
	this->shootProcess();

	// If health is 0, play death sound and set deleted status.
	if (getHealth() <= 0){
		// Play Death Sound
		resourceHandler->getSound(Constants::ResourceC::Sound::FX_ENEMY_DEATH).play();
		setDeleted(true);
	}
}


/// <summary>
/// Gets the repeat.
/// </summary>
/// <returns>Repeat flag</returns>
int Enemy::getRepeat() const
{
	return this->repeat;
}

/// <summary>
/// Sets the type of the enemy.
/// </summary>
/// <param name="type">The enemy type.</param>
void Enemy::setEnemyType(Constants::EnemyC::Type type)
{
	this->enemyType = type;
}

/// <summary>
/// Gets the type of the enemy.
/// </summary>
/// <returns>Returns the enemy type</returns>
Constants::EnemyC::Type Enemy::getEnemyType()
{
	return this->enemyType;
}

int Enemy::getDelay() const {
    return spawnDelay;
}
