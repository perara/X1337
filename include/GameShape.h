#include <SFML\Graphics.hpp>
#include <map>

class GameShape : public sf::ConvexShape
{
public:
	const enum ShapeType{
		CIRCLE,
		TRIANGLE,
		STARSHIP,
		BOSS,
		PLAYER_SHIP,
		SQUARE,
		SHAPECOUNT
	};

	GameShape(ShapeType shapeType); // Other
	GameShape(GameShape::ShapeType shapeType, float size); // Triangle
	GameShape(ShapeType shapeType, int radius, int pointCount = 5); // Circle

	ShapeType getShapeType();
	sf::Time timeStep;
	int getRadius(); // Only valid for circle
private:
	ShapeType shapeType;
	void setCircleShape(double radius, sf::Vector2f center);
	void setTriangleShape(float size);
	std::map<std::string, float> properties;
};