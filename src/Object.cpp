#include "../include/Object.h"
#include "../include/Log.h"
#include "../include/GameShape.h"
#include <cmath>
#include <cfloat>

/// <summary>
/// Draws this Object.
/// </summary>
void Object::draw()
{
	this->window.draw(*this->sprite);

	// Bounding boxes.
	/*sf::RectangleShape();
	sf::FloatRect bounds = this->sprite->getGlobalBounds();
	sf::RectangleShape af(sf::Vector2f(bounds.width,bounds.height));
	af.setPosition(bounds.left, bounds.top);
	af.setOutlineColor(sf::Color(141,23,22,23));
	af.setFillColor(sf::Color(255,255,255,150));

	this->window.draw(af);*/

}
/// <summary>
/// Processes the object
/// </summary>
void Object::process(){}

/// <summary>
/// Get the deleted flag
/// </summary>
/// <returns>Returns bool with deleted status</returns>
bool Object::getDeleted() const{
	return this->deleted;
}

/// <summary>
/// Sets the deleted flag
/// </summary>
/// <param name="del">The delete flag</param>
void Object::setDeleted(bool del)
{
	deleted = del;
}


/// <summary>
/// Determines whether [is out of bounds].
/// </summary>
/// <returns></returns>
bool Object::isOutOfBounds()
{
	// Checks weither a object is out of bounds (out of screen)
	if (this->sprite->getPosition().x > window.getView().getSize().x ||
		this->sprite->getPosition().x < 0 ||
		this->sprite->getPosition().y > window.getView().getSize().y ||
		this->sprite->getPosition().y < 0){
		return true;
	}
	else{
		return false;
	}
}

/// <summary>
/// Finalizes an instance of the <see cref="Object"/> class.
/// </summary>
Object::~Object()
{
	LOGD("Object Deconstructor called");
}

/// <summary>
/// Superplan Axis Theorem, this function takes in two game shapes and checks if they collide. This only works on CONVEX polygons.
/// </summary>
/// <param name="c1">First GameShape.</param>
/// <param name="c2">Second GameShape</param>
/// <returns>Intersection flag (intersects or not)</returns>
bool Object::sat(const std::shared_ptr<GameShape>& c1, const std::shared_ptr<GameShape>& c2)
{
	float c1_tx = c1->getPosition().x;
    float c1_ty = c1->getPosition().y;

    float c2_tx = c2->getPosition().x;
    float c2_ty = c2->getPosition().y;


	bool ret = true;

    unsigned long c1_faces = c1->getPointCount();
    unsigned long  c2_faces = c2->getPointCount();

	//For each of the points in c1
	for (int i = 0; i < c1_faces; i++)
	{
		// Get point from iterator
		float fx = c1->getPoint(i).x - c1->getPoint((i + 1) % c1_faces).x;
		float fy = c1->getPoint(i).y - c1->getPoint((i + 1) % c1_faces).y;

		// Create a perpendicular axis to project on (axis x, axis y)
		float ax = -fy, ay = fx;

		// Normalize axis
		float len_v = std::sqrt(ax * ax + ay * ay);
		ax /= len_v;
		ay /= len_v;

		//Carve out the min and max values
		//float c1_min = std::numeric_limits<float>::max(), c1_max = -c1_min;
		//float c2_min = std::numeric_limits<float>::max(), c2_max = -c2_min;
		float c1_min = FLT_MAX, c1_max = -FLT_MAX;
		float c2_min = FLT_MAX, c2_max = -FLT_MAX;

		//Project every point in c1 on the axis and store min and max
		for (int j = 0; j < c1_faces; j++)
		{
			float c1_proj = (ax * (c1->getPoint(j).x + c1_tx) + ay * (c1->getPoint(j).y + c1_ty)) / (ax * ax + ay * ay);
			c1_min = std::min(c1_proj, c1_min);
			c1_max = std::max(c1_proj, c1_max);
		}

		//Project every point in c2 on the axis and store min and max
		for (int j = 0; j < c2_faces; j++)
		{
			float c2_proj = (ax * (c2->getPoint(j).x + c2_tx) + ay * (c2->getPoint(j).y + c2_ty)) / (ax * ax + ay * ay);
			c2_min = std::min(c2_proj, c2_min);
			c2_max = std::max(c2_proj, c2_max);
		}

		//Return if the projections do not overlap
		if (!(c1_max >= c2_min && c1_min <= c2_max))
			ret = false; //return false;
	}

	return ret; //return true;
}

