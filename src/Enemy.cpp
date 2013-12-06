#include "Enemy.h"
#include "GameShape.h"
#include "BulletFactory.h"
#include "Bullet.h"
#include "Log.h"

Enemy::Enemy(sf::RenderWindow& window,
	std::queue<sf::Vector3f> path,
	int type, int repeat, BulletFactory& bFactory,
	std::list<std::unique_ptr<Bullet>>& bullets,
	std::unique_ptr<ResourceHandler>& resourceHandler,
	const sf::Time& timeStep
	) :
	Shooter(window, bFactory, bullets, resourceHandler, timeStep)
{
	this->setType(Shooter::ShooterType::ENEMY);
	this->pathTemplate = path; // A qeueu which should not be touched (This is used to refill old queue
	this->enemyClock.restart();
	this->setRepeat(repeat);
	if (type == 1) // REGULAR
	{
		setHealth(2);
		sprite = std::unique_ptr<GameShape>(new GameShape(GameShape::ShapeType::STARSHIP));
		sprite->setTexture(&resourceHandler->getTexture(ResourceHandler::Texture::ENEMY_SHIP));
		this->setEnemyType(Enemy::EnemyType::REGULAR);
	}
	else if (type == 2) // Chubby Mob
	{
		setHealth(10);
		sprite = std::unique_ptr<GameShape>(new GameShape(GameShape::ShapeType::CIRCLE, 40, 30));
		sprite->setTexture(&resourceHandler->getTexture(ResourceHandler::Texture::CHUBBY_SHIP_TEXTURE));
		this->setEnemyType(Enemy::EnemyType::CHUBBY);

	}
	else if (type == 3) // BOSS
	{
		setHealth(250);
		sprite = std::unique_ptr<GameShape>(new GameShape(GameShape::ShapeType::BOSS));
		sprite->setTexture(&resourceHandler->getTexture(ResourceHandler::Texture::BOSS));
		this->setEnemyType(Enemy::EnemyType::BOSS);

	}
	else if (type == 4) // Umbasta boss
	{
		setHealth(750);
		sprite = std::unique_ptr<GameShape>(new GameShape(GameShape::ShapeType::CIRCLE, 80, 30));
		sprite->setTexture(&resourceHandler->getTexture(ResourceHandler::Texture::BOSS_DEATHSTAR_TEXTURE));
		this->setEnemyType(Enemy::EnemyType::DEATHSTAR);
	}

#ifdef _DEBUG
	sprite->setOutlineThickness(1);
	sprite->setOutlineColor(sf::Color::Cyan);
#endif

	setInitPath();
}

void Enemy::setInitPath()
{

	this->deleted = false;
	this->path = pathTemplate;
	this->currentPath = this->path.front();
	this->path.pop();

	this->sprite->setPosition(currentPath.x, currentPath.y);


}

Enemy::~Enemy(){
	LOGD("Deconstructor called for: Enemy#" << this);
}


void Enemy::shoot(int shoot)
{
	if (shoot != -1 && shoot != 0)
	{
		if (getEnemyType() == Enemy::EnemyType::REGULAR)
		{
			std::unique_ptr<Bullet> b = getBulletFactory().requestObject(Bullet::Type::standardShot);
			b->setOwner(this->getType());
			b->sprite->setPosition(this->sprite->getPosition().x, this->sprite->getPosition().y - 10);
			getBullets().push_back(std::move(b));

		}
		else if (getEnemyType() == Enemy::EnemyType::CHUBBY)
		{
			std::list<std::unique_ptr<Bullet>> bat = getBulletFactory().requestBatch(3, Bullet::Type::standardShot);
			int startX = (this->sprite->getPosition().x) - (this->sprite->getGlobalBounds().width / 2) + 10;

			for (auto& i : bat)
			{
				std::unique_ptr<Bullet> bs = std::move(i);
				bs->setOwner(this->getType());
				bs->sprite->setPosition(startX, this->sprite->getPosition().y - 10);
				getBullets().push_back(std::move(bs));
				startX += this->sprite->getGlobalBounds().width / 4;
			}

		}
		else if (getEnemyType() == Enemy::EnemyType::BOSS)
		{
			std::list<std::unique_ptr<Bullet>> bat = getBulletFactory().requestBatch(10, Bullet::Type::standardShot);
			int startX = (this->sprite->getPosition().x) - (this->sprite->getGlobalBounds().width / 2);

			for (auto& i : bat)
			{
				std::unique_ptr<Bullet> bs = std::move(i);
				bs->setOwner(this->getType());
				bs->sprite->setPosition(startX, this->sprite->getPosition().y - 10);
				getBullets().push_back(std::move(bs));
				startX += this->sprite->getGlobalBounds().width / 10;
			}

		}
		else if (getEnemyType() == Enemy::EnemyType::DEATHSTAR)
		{
			// Circular shoot pattern
			if (shoot == 3) // Special attack
			{
				std::unique_ptr<Bullet> b = getBulletFactory().requestObject(Bullet::Type::standardShot);
				b->setOwner(this->getType());
				b->setSpeed(sf::Vector2f(0.0f, 350.0f));
				b->sprite->setFillColor(sf::Color::Green);
				b->sprite->setPosition(this->sprite->getPosition().x, this->sprite->getPosition().y - 10);
				getBullets().push_back(std::move(b));
			}
			else
			{
				for (int i = 0; i < 360; i += 2)
				{
					std::unique_ptr<Bullet> b = getBulletFactory().requestObject(Bullet::Type::standardShot);
					b->setOwner(this->getType());
					b->setRotation(i, sf::Vector2f(150, 150));
					b->sprite->setPosition(this->sprite->getPosition().x, this->sprite->getPosition().y);
					getBullets().push_back(std::move(b));
				}
			}
		}
	}

}

void Enemy::process()
{
	this->hitDetection();

	// Start
	sf::Vector2f length;
	length.x = abs(this->currentPath.x - path.front().x);
	length.y = abs(this->currentPath.y - path.front().y);

	// End
	sf::Vector2f currentPosition;
	currentPosition.x = abs(this->currentPath.x - this->sprite->getPosition().x);
	currentPosition.y = abs(this->currentPath.y - this->sprite->getPosition().y);



	float dx = this->path.front().x - this->currentPath.x;
	float dy = this->path.front().y - this->currentPath.y;
	float len = sqrtf(dx * dx + dy * dy);

	dx = (dx / len) * timeStep.asSeconds() * 50;
	dy = (dy / len) * timeStep.asSeconds() * 50;

	// Normal Shoot processing
	if (this->enemyClock.getElapsedTime().asMilliseconds() > 400)
	{
		this->shoot(this->currentPath.z);
		this->enemyClock.restart();
	}

	// Special Cases shoot processing (Like deathstar lazer)
	if (this->currentPath.z == 3) // check if its a special case (4 is special attack for deathstar)
	{
		this->shoot(this->currentPath.z);
	}


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
	else
	{
		//std::cout << currentPath.x << ","<< currentPath.y << "->" << this->path->front().x << "," << this->path->front().y << std::endl;
		if (path.size() > 1)
		{
			currentPath = path.front();
			path.pop();


		}
		else
		{
			LOGD("Enemy#" << this << " delete flag set");
			this->deleted = true;
			if (this->getRepeat() == 1){
				setInitPath();
			}

		}

	}

}


int Enemy::getRepeat()
{
	return this->repeat;
}
void Enemy::setRepeat(int rep)
{
	this->repeat = rep;
}

void Enemy::setEnemyType(Enemy::EnemyType type)
{
	this->enemyType = type;
}

Enemy::EnemyType Enemy::getEnemyType()
{
	return this->enemyType;
}
