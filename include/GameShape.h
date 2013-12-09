#include <SFML\Graphics.hpp>
#include <map>

/// <summary>
/// GameShape is a class which is derived from sf::ConvexShape, this class can represent all convex shapes.
/// </summary>
class GameShape : public sf::ConvexShape
{
public:

	/// ShapeType is an enumerable which contains all of the defined convex shapes in the GameShape implementation
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

	int getRadius(); // Only valid for circle
private:
	ShapeType shapeType;
	void setCircleShape(double radius, sf::Vector2f center);
	void setTriangleShape(float size);
	std::map<std::string, float> properties;
};