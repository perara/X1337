#include "../include/GameShape.h"
#include "../include/Log.h"
#include "../include/Constants.h"
#include <cmath>

// Other Default
/// <summary>
/// Initializes a new instance of the <see cref="GameShape"/> class.
/// </summary>
/// <param name="shapeType">Type of the shape constructed</param>
GameShape::GameShape(Constants::GameShapeC::Type shapeType) :
shapeType(shapeType)
{
	// Scale value for the size, this may be moved the the constructor.
	float scale = 0.50f;

	// Defines each of the polygons for each for the shapes.
	switch (shapeType)
	{
	case Constants::GameShapeC::Type::TRIANGLE:
		this->setPointCount(3);
		setTriangleShape(20);
		break;
	case Constants::GameShapeC::Type::STARSHIP:
		this->setPointCount(11);
		this->setPoint(0, sf::Vector2f(-20, -10));
		this->setPoint(1, sf::Vector2f(-30, -10));
		this->setPoint(2, sf::Vector2f(-20, -20));
		this->setPoint(3, sf::Vector2f(-20, -30));
		this->setPoint(4, sf::Vector2f(-10, -40));
		this->setPoint(5, sf::Vector2f(10, -40));
		this->setPoint(6, sf::Vector2f(20, -30));
		this->setPoint(7, sf::Vector2f(20, -20));
		this->setPoint(8, sf::Vector2f(30, -10));
		this->setPoint(9, sf::Vector2f(20, -10));
		break;
	case Constants::GameShapeC::Type::PLAYER_SHIP:
		this->setPointCount(9);
		this->setPoint(0, sf::Vector2f(0, -20)* scale);
		this->setPoint(1, sf::Vector2f(-20, 5)* scale);
		this->setPoint(2, sf::Vector2f(-20, 25)* scale);
		this->setPoint(3, sf::Vector2f(-10, 25)* scale);
		this->setPoint(4, sf::Vector2f(-4, 15)* scale);
		this->setPoint(5, sf::Vector2f(4, 15)* scale);
		this->setPoint(6, sf::Vector2f(10, 25)* scale);
		this->setPoint(7, sf::Vector2f(20, 25)* scale);
		this->setPoint(8, sf::Vector2f(20, 5)* scale);
		break;
	case Constants::GameShapeC::Type::BOSS:
		this->setPointCount(8);
		setCircleShape(80, sf::Vector2f(0, 0));
		break;
	default:
		LOGE("There is no shapetype defined for this constructor which maches your type");
		break;
	};

}

// Triangle
/// <summary>
/// Initializes a new instance of the <see cref="GameShape"/> class.
/// </summary>
/// <param name="shapeType">Define shape type for the shape.</param>
/// <param name="size">Defines a size for the size (scale)</param>
GameShape::GameShape(Constants::GameShapeC::Type shapeType, float size) :
shapeType(shapeType)
{
	switch (shapeType)
	{
	case Constants::GameShapeC::Type::TRIANGLE:
		this->setPointCount(3);
		setTriangleShape(size);
		break;
	default:
		LOGE("There is no shapetype defined for this constructor which maches your type");
		break;
	};

}



/* Circle or shapes with radius */
/// <summary>
/// Initializes a new instance of the <see cref="GameShape"/> class. (circle)
/// </summary>
/// <param name="shapeType">Shape definiton, this is usually a circle for this constructor..</param>
/// <param name="radius">The Circle radius</param>
/// <param name="pointCount">The point count (polygon).</param>
GameShape::GameShape(Constants::GameShapeC::Type shapeType, float radius, int pointCount) :
shapeType(shapeType)
{
	switch (shapeType)
	{
	case Constants::GameShapeC::Type::CIRCLE:
		this->setPointCount(pointCount);
		this->properties["radius"] = radius;
		setCircleShape(radius, sf::Vector2f(0, 0)); // TODO
		break;
	};


}


/// <summary>
/// Returns the shapetype for current shape.
/// </summary>
/// <returns>Shape yupe</returns>
Constants::GameShapeC::Type GameShape::getShapeType()
{
	return shapeType;
}

/// <summary>
/// Sets the shape to a triangle shape.
/// </summary>
/// <param name="size">Scale Value</param>
void GameShape::setTriangleShape(float size)
{
	this->setPoint(0, sf::Vector2f(-0.5, 0)*size);
	this->setPoint(1, sf::Vector2f(0.5, 0)*size);
	this->setPoint(2, sf::Vector2f(0, -1)*size);

}


/// <summary>
/// Defines this shapes as a circle
/// </summary>
/// <param name="radius">Circle radius</param>
/// <param name="center">Center of the circle.</param>
void GameShape::setCircleShape(float radius, sf::Vector2f center)
{
	float PI = 3.14159265358979323846;
	float slice = 2 * PI / (float)this->getPointCount();

	for (int i = 0; i < this->getPointCount(); i++)
	{

		float angle = slice * (float)i;
		float newX = (center.x + radius * std::cos(angle));
		float newY = (center.y + radius * std::sin(angle));

		sf::Vector2f p = sf::Vector2f(newX, newY);
		this->setPoint(i, p);
	}

}

/// <summary>
/// Gets the radius.
/// </summary>
/// <returns>Returns radius of a shape, -1 if shape is not a circle (not valid for radius)</returns>
float GameShape::getRadius(){
	if (this->shapeType == Constants::GameShapeC::Type::CIRCLE){
		return this->properties["radius"];
	}
	else{
		return this->getGlobalBounds().width / 2;
	}
}