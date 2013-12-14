#include "Shooter.h"
#include "GameShape.h"
#include "BulletFactory.h"
#include "Log.h"

/// <summary>
/// Initializes a new instance of the <see cref="Shooter"/> class.
/// </summary>
/// <param name="window">An renderwindow reference</param>
/// <param name="bFactory">An bulletfactory reference.</param>
/// <param name="bullets">The bulletslist</param>
/// <param name="resourceHandler">Resourcehandler</param>
/// <param name="timeStep">The time step.</param>
Shooter::Shooter(sf::RenderWindow& window,
	BulletFactory& bFactory,
	std::list<std::unique_ptr<Bullet>>& bullets,
	std::shared_ptr<ResourceHandler>& resourceHandler,
	const sf::Time& timeStep) :
	Object(window),
	bullets(bullets),
	bFactory(bFactory),
	resourceHandler(resourceHandler),
	timeStep(timeStep),
	godMode(false)

{}

/// <summary>
/// Sets the score value of the shooter
/// </summary>
/// <param name="value">The value.</param>
void Shooter::setScoreValue(float value)
{
	scoreValue = value;
}

/// <summary>
/// Retrieves the score value.
/// </summary>
/// <returns>Score value as an float</returns>
float Shooter::getScoreValue()
{
	return scoreValue;
}

/// <summary>
/// Sets the shooter health.
/// </summary>
/// <param name="value">The health value to set it to.</param>
void Shooter::setHealth(int value)
{
	if (godMode) return;
	health = value;
}

/// <summary>
/// Sets the start health.
/// </summary>
/// <param name="value">The value of health to set it to.</param>
void Shooter::setStartHealth(int value)
{
	this->startHealth = value;
}


/// <summary>
/// Retrieves the start health.
/// </summary>
/// <returns>Start health</returns>
const int Shooter::getStartHealth()
{
	return startHealth;
}


/// <summary>
/// Retrieves the health.
/// </summary>
/// <returns>Current Health value</returns>
const int Shooter::getHealth()
{
	return health;
}

/// <summary>
/// A function which is processed each frame, the function checks for intersection between the bullet list.
/// </summary>
void Shooter::hitDetection()
{
	// Check if the bullet list is empty, continiue if its not empty.
	if (!getBullets().empty())
	{
		// Set default hit bool to false
		bool wasHit = false;

		// Iterate through each of the bullets
		for (auto& i : getBullets())
		{
			// Ignore collision detections if the type is same as the bullet owner type
			if (getType() == i->getOwner()) continue;

			// Checks if the bullet is to far away to be able to the ship
			int distDx = abs(sprite->getPosition().x - i->sprite->getPosition().x);
			int distDy = abs(sprite->getPosition().y - i->sprite->getPosition().y);
			if (!(distDx + 5 < sprite->getGlobalBounds().width) || !(distDy + 5 < sprite->getGlobalBounds().width)) continue;

			// Do collision detection
			wasHit = sat(sprite, i->sprite);

			// If the object was hit
			if (wasHit)
			{
				// Decrement health with the bullet damage
				setHealth(getHealth() - i->getBulletType());

				// Set bullet to deleted.
				i->setDeleted(true);

				// Check if the ENEMY is dead
				if (getHealth() <= 0 && getType() != Shooter::ShooterType::PLAYER)
				{
					// Play deat hsound
					resourceHandler->getSound(ResourceHandler::Sound::FX_ENEMY_DEATH).play();

					// Set deleted flag
					setDeleted(true);
				}
			}
		}
	}
}

/// <summary>
/// Gets the type of the shooter
/// </summary>
/// <returns>THIS shooterType</returns>
Shooter::ShooterType Shooter::getType()
{
	return this->shooterType;
}

/// <summary>
/// Gets the bullet factory.
/// </summary>
/// <returns></returns>
BulletFactory& Shooter::getBulletFactory()
{
	return bFactory;
}

/// <summary>
/// Gets the bullets.
/// </summary>
/// <returns>The bullet factory (This should be removed)</returns>
std::list<std::unique_ptr<Bullet>>& Shooter::getBullets()
{
	return this->bullets;
}
