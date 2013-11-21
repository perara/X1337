#include <SFML\Graphics\ConvexShape.hpp>
#include <map>
#include <string>
#include "Log.h"

class GameShape : public sf::ConvexShape
{
public:
	const enum ShapeType{
		circle = 0,
		triangle = 1,
		square = 2, 
	};

	GameShape(ShapeType shapeType);
	GameShape(ShapeType shapeType, int radius, int pointCount = 10);

	ShapeType getShapeType();

	int getRadius(); // Only valid for circle
protected:
	ShapeType shapeType;
	void setCircleShape(double radius, sf::Vector2f center);
	std::map<std::string, float> properties;
};