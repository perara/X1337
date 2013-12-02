#include "Shooter.h"
#include "GameShape.h"
#include "BulletFactory.h"
#include "Log.h"

Shooter::Shooter(sf::RenderWindow& window, 
				 BulletFactory& bFactory,   
				 std::list<std::unique_ptr<Bullet>>& bullets, 
				 std::unique_ptr<ResourceHandler>& resourceHandler,
				 const sf::Time& timeStep): 
Object(window),
	bullets(bullets),
	bFactory(bFactory),
	resourceHandler(resourceHandler),
	timeStep(timeStep)

{}

void Shooter::setScoreValue(float value)
{
	scoreValue=value;
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



bool Shooter::sat(std::shared_ptr<GameShape> c1, std::shared_ptr<GameShape> c2)
{
	int c1_tx = c1->getPosition().x;
	int c1_ty = c1->getPosition().y;

	int c2_tx = c2->getPosition().x;
	int c2_ty = c2->getPosition().y;


	bool ret = true;

	int c1_faces = c1->getPointCount();
	int c2_faces = c2->getPointCount();

	//For each of the points in c1
	for(int i = 0; i < c1_faces; i++)
	{
		// Get point from iterator
		float fx = c1->getPoint(i).x - c1->getPoint((i + 1) % c1_faces).x;
		float fy = c1->getPoint(i).y - c1->getPoint((i + 1) % c1_faces).y;

		// Create a perpendicular axis to project on (axis x, axis y)
		float ax = -fy, ay = fx;

		// Normalize axis
		float len_v = sqrt(ax * ax + ay * ay);
		ax /= len_v;
		ay /= len_v;

		//Carve out the min and max values
		//float c1_min = std::numeric_limits<float>::max(), c1_max = -c1_min;
		//float c2_min = std::numeric_limits<float>::max(), c2_max = -c2_min;
		float c1_min = FLT_MAX, c1_max = -FLT_MAX;
		float c2_min = FLT_MAX, c2_max = -FLT_MAX;

		//Project every point in c1 on the axis and store min and max
		for(int j = 0; j < c1_faces; j++)
		{
			float c1_proj = (ax * (c1->getPoint(j).x + c1_tx) + ay * (c1->getPoint(j).y + c1_ty)) / (ax * ax + ay * ay);
			c1_min = std::min(c1_proj, c1_min);
			c1_max = std::max(c1_proj, c1_max);
		}

		//Project every point in c2 on the axis and store min and max
		for(int j = 0; j < c2_faces; j++)
		{
			float c2_proj = (ax * (c2->getPoint(j).x + c2_tx) + ay * (c2->getPoint(j).y + c2_ty)) / (ax * ax + ay * ay);
			c2_min = std::min(c2_proj, c2_min);
			c2_max = std::max(c2_proj, c2_max);
		}

		//Return if the projections do not overlap
		if(!(c1_max >= c2_min && c1_min <= c2_max))
			ret = false; //return false;
	}

	return ret; //return true;
}


void Shooter::hitDetection()
{
	if(!getBullets().empty())
	{
		bool wasHit = false;
		for(auto& i: getBullets())
		{
			if(getType() == i->getOwner()) continue; // Dont compute bullets for your own type

			wasHit = sat(sprite, i->sprite);

			if(wasHit)
			{
				setHealth(getHealth() - i->getBulletType());
				i->setDeleted(true);


				// KILL IF DEAD
				if(getHealth() <= 0 && getType() != Shooter::ShooterType::PLAYER)
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
