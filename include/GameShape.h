#include <SFML/Graphics.hpp>
#include <map>
#include "Constants.h"

/// <summary>
/// GameShape is a class which is derived from sf::ConvexShape, this class can represent all convex shapes.
/// </summary>
class GameShape : public sf::ConvexShape
{
public:



	GameShape(Constants::GameShapeC::Type shapeType); // Other
	GameShape(Constants::GameShapeC::Type shapeType, float size); // Triangle
	GameShape(Constants::GameShapeC::Type shapeType, float radius, int pointCount); // Circle

    Constants::GameShapeC::Type getShapeType();

	float getRadius(); // Only valid for circle
private:
    Constants::GameShapeC::Type shapeType;
	void setCircleShape(float radius, sf::Vector2f center);
	void setTriangleShape(float size);
	std::map<std::string, float> properties;
};