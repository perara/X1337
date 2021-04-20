#include "../include/Shooter.h"
#include "../include/GameShape.h"
#include "../include/BulletFactory.h"
#include "../include/Log.h"

/// <summary>
/// Initializes a new instance of the <see cref="Shooter"/> class.
/// </summary>
/// <param name="window">An renderwindow reference</param>
/// <param name="bFactory">An bulletfactory reference.</param>
/// <param name="bullets">The bulletslist</param>
/// <param name="resourceHandler">Resourcehandler</param>
/// <param name="timeStep">The time step.</param>
Shooter::Shooter(Renderer& window,
	BulletFactory& bFactory,
	std::list<std::unique_ptr<Bullet>>& bullets,
	std::shared_ptr<ResourceManager>& resourceHandler,
    std::list<std::shared_ptr<Shooter>>& objects,
	const sf::Time& timeStep) :
	Object(window),
	resourceHandler(resourceHandler),
	timeStep(timeStep),
    objects(objects),
    bullets(bullets),
    bFactory(bFactory),
    totalDamageDone(0)
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
float Shooter::getScoreValue() const
{
	return scoreValue;
}

/// <summary>
/// Sets the shooter health.
/// </summary>
/// <param name="value">The health value to set it to.</param>
void Shooter::setHealth(int value)
{
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
int Shooter::getStartHealth() const
{
	return startHealth;
}


/// <summary>
/// Retrieves the health.
/// </summary>
/// <returns>Current Health value</returns>
int Shooter::getHealth() const
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
		bool wasHit;

		// Iterate through each of the bullets
		for (auto& i : getBullets())
		{
			// Ignore collision detections if the type is same as the bullet owner type
			if (this->getType() == i->getOwner()->getType()) continue;

			// Checks if the bullet is to far away to be able to the ship
			float distDx = std::abs(sprite->getPosition().x - i->sprite->getPosition().x);
            float distDy = std::abs(sprite->getPosition().y - i->sprite->getPosition().y);
			if (distDx + 5 >= sprite->getGlobalBounds().width || distDy + 5 >= sprite->getGlobalBounds().width) continue;

			// Do collision detection
			wasHit = sat(sprite, i->sprite);

			// If the object was hit
			if (wasHit)
			{
				// Decrement health with the bullet damage
				auto bulletDamage = i->getBulletType();
				setHealth(getHealth() - bulletDamage);
				i->getOwner()->addTotalDamageDone(bulletDamage);

				// Set bullet to deleted.
				i->setDeleted(true);
			}
		}
	}
}

void Shooter::processShipCollision(){
    // Ship collision
    for (auto& i : objects)
    {
        if (i->getType() == Constants::ShooterType::PLAYER) continue;

        if (Shooter::sat(i->sprite, this->sprite))
        {
            // Kill the player, (ship collisions never end well)
            this->setHealth(this->getHealth() - 1);
            i->setHealth(i->getHealth() - 1);
            addTotalDamageDone(1);
            i->addTotalDamageDone(1);
        }
    }
};

/// <summary>
/// Gets the type of the shooter
/// </summary>
/// <returns>THIS shooterType</returns>
Constants::ShooterType Shooter::getType()
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

void Shooter::addTotalDamageDone(int damage) {
    totalDamageDone += damage;
}

int Shooter::getTotalDamageDone() const{
    return totalDamageDone;
}
