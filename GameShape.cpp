#include "GameShape.h"


GameShape::GameShape(GameShape::ShapeType shapeType):
	shapeType(shapeType)
{

	switch(shapeType)
	{
	case GameShape::ShapeType::triangle:
		break;
	case GameShape::ShapeType::square:
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
	case GameShape::ShapeType::circle:
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
	if(this->shapeType  == GameShape::ShapeType::circle){
		return this->properties["radius"];
	}else{
		return -1;
	}
}