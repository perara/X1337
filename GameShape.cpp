#include "GameShape.h"
#include "Log.h"

// Other Default
GameShape::GameShape(GameShape::ShapeType shapeType):
	shapeType(shapeType)
{

	switch(shapeType)
	{
	case GameShape::ShapeType::TRIANGLE:
		this->setPointCount(3);
		setTriangleShape(20);
		break;
	case GameShape::ShapeType::STARSHIP:
		this->setPointCount(8);

		this->setPoint(0, sf::Vector2f(-30,-10));
		this->setPoint(1, sf::Vector2f(-20,-20));
		this->setPoint(2, sf::Vector2f(-20,-30));
		this->setPoint(3, sf::Vector2f(-10,-40));
		this->setPoint(4, sf::Vector2f(10,-40));
		this->setPoint(5, sf::Vector2f(20,-30));
		this->setPoint(6, sf::Vector2f(20,-20));
		this->setPoint(7, sf::Vector2f(30,-10));

		break;
	case GameShape::ShapeType::BOSS:
		this->setPointCount(20);
		setCircleShape(80, sf::Vector2f(0,0));

		break;
	default:
		LOGE("There is no shapetype defined for this constructor which maches your type");
		break;
	};

}

// Triangle
GameShape::GameShape(GameShape::ShapeType shapeType, float size):
	shapeType(shapeType)
{
	switch(shapeType)
	{
	case GameShape::ShapeType::TRIANGLE:
		this->setPointCount(3);
		setTriangleShape(size);
		break;
	default:
		LOGE("There is no shapetype defined for this constructor which maches your type");
		break;
	};

}



/* Circle or shapes with radius */
GameShape::GameShape(GameShape::ShapeType shapeType, int radius, int pointCount):
	shapeType(shapeType)
{
	switch(shapeType)
	{
	case GameShape::ShapeType::CIRCLE:
		this->setPointCount(pointCount);
		this->properties["radius"] = radius;
		setCircleShape(radius, sf::Vector2f(0,0)); // TODO
		break;
	};


}


GameShape::ShapeType GameShape::getShapeType()
{
	return shapeType;
}

void GameShape::setTriangleShape(float size)
{
	this->setPoint(0, sf::Vector2f(-0.5,0)*size);
	this->setPoint(1, sf::Vector2f(0.5,0)*size);
	this->setPoint(2, sf::Vector2f(0,-1)*size);

}


void GameShape::setCircleShape(double radius, sf::Vector2f center)
{
	float PI = 3.14159265358979323846;
	double slice = 2 * PI / this->getPointCount();

	for (int i = 0; i < this->getPointCount(); i++)
	{

		double angle = slice * i;
		float newX = (center.x + radius * cos(angle));
		float newY = (center.y + radius * sin(angle));

		sf::Vector2f p = sf::Vector2f(newX, newY);
		this->setPoint(i, p);
	}

}

// Returns radius of a shape, -1 if shape is not a circle (not valid for radius)
int GameShape::getRadius(){
	if(this->shapeType  == GameShape::ShapeType::CIRCLE){
		return this->properties["radius"];
	}else{
		return -1;
	}
}