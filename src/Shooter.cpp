#include "Shooter.h"
#include "GameShape.h"
#include "BulletFactory.h"
#include "Log.h"

Shooter::Shooter(sf::RenderWindow& window,
	BulletFactory& bFactory,
	std::list<std::unique_ptr<Bullet>>& bullets,
	std::unique_ptr<ResourceHandler>& resourceHandler,
	const sf::Time& timeStep) :
	Object(window),
	bullets(bullets),
	bFactory(bFactory),
	resourceHandler(resourceHandler),
	timeStep(timeStep)

{}

void Shooter::setScoreValue(float value)
{
	scoreValue = value;
}

float Shooter::getScoreValue()
{
	return scoreValue;
}

void Shooter::setHealth(int value)
{
	health = value;
}

const int Shooter::getHealth()
{
	return health;
}

void Shooter::hitDetection()
{
	if (!getBullets().empty())
	{
		bool wasHit = false;
		for (auto& i : getBullets())
		{
			if (getType() == i->getOwner()) continue; // Dont compute bullets for your own type

			wasHit = sat(sprite, i->sprite);

			if (wasHit)
			{
				setHealth(getHealth() - i->getBulletType());
				i->setDeleted(true);


				// KILL IF DEAD
				if (getHealth() <= 0 && getType() != Shooter::ShooterType::PLAYER)
				{
					resourceHandler->getSound(ResourceHandler::Sound::ENEMY_DEATH).play();
					deleted = true;
				}

			}
		}
	}
}

void Shooter::setType(Shooter::ShooterType shooterType)
{
	this->shooterType = shooterType;
}

Shooter::ShooterType Shooter::getType()
{
	return this->shooterType;
}


// BulletFactory Getter/Setter
BulletFactory& Shooter::getBulletFactory()
{
	return bFactory;
}


// Bullets getter/setter
std::list<std::unique_ptr<Bullet>>& Shooter::getBullets()
{
	return this->bullets;
}
