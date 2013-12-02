#pragma once
#include <SFML\Graphics.hpp>
#include <vector>
#include "Log.h"


template<class T>
class QuadTree
{
public:

	QuadTree(sf::FloatRect bounds, int level, int MAX_OBJECTS, int MAX_LEVEL):
		MAX_LEVEL(MAX_LEVEL),
		MAX_OBJECTS(MAX_OBJECTS),
		bounds(bounds),
		level(level)
	{
		LOGD("Created tree level: " << level << " with bounds: " << bounds.left << "," << bounds.top << "," << bounds.width  << "," << bounds.height << " |QuadTree#" << this);

		// Set tree's to null
		NE = nullptr;
		NW = nullptr;
		SE = nullptr;
		SW = nullptr;

	};



	void splitQuad()
	{
		if(level >= MAX_LEVEL)
		{
			LOGD("Error: You have exceeded max level of the tree, Its overflowing!, Increase MAX_LEVEL or MAX_OBJECTS");
			return;
		}


		// Ensure that all tree's are nullptr before creating new ones.
		if(
			NE == nullptr &&
			NW == nullptr &&
			SE == nullptr && 
			SW == nullptr
			)
		{
			sf::Vector2f subWindowSize((bounds.width / 2), (bounds.height / 2));

			// Set bounds for each of the new quadrants
			sf::FloatRect rectNE((bounds.width / 2), 0, subWindowSize.x, subWindowSize.y);
			sf::FloatRect rectNW(0, 0, subWindowSize.x, subWindowSize.y);
			sf::FloatRect rectSE((bounds.width / 2), (bounds.height / 2), subWindowSize.x, subWindowSize.y);
			sf::FloatRect rectSW(0, (bounds.height / 2), subWindowSize.x, subWindowSize.y);

			// If any of the rects is less than 1 pixel we have to throw a error
			if(rectNE.width < 1 || rectNE.height < 1)
			{
				LOGD("Error: Quandrants cannot be less than 1 pixel, decrease MAX_OBJECTS");
				return;
			}

			// Divide into 4 new quadrants
			NE = new QuadTree<T>(rectNE, this->level + 1, MAX_OBJECTS, MAX_LEVEL); // NE
			NW = new QuadTree<T>(rectNW, this->level + 1, MAX_OBJECTS, MAX_LEVEL); // NW
			SE = new QuadTree<T>(rectSE, this->level + 1, MAX_OBJECTS, MAX_LEVEL); // SE
			SW = new QuadTree<T>(rectSW, this->level + 1, MAX_OBJECTS, MAX_LEVEL); // SW
		}
		else
		{
			LOGD("Error: Could not create a new subTree because one of the NE, NW, SE, SW tree's was not null!");
		}

	};


	bool insert(T* obj, sf::FloatRect objBounds)
	{
		// Check if the pointer given is nullptr
		if(obj == nullptr)
		{
			LOGD("You cannot add a object which is nullptr!");
		}

		// Get positon of the object
		sf::Vector2f pt(objBounds.left, objBounds.top);


	/*	if(this->bounds.contains(pt))
		{
			LOGD("Point:" <<pt.x << "," << pt.x << "Remember: " << (objects.size() < MAX_OBJECTS));
			LOGI("Bounds(x,y,w,h): " << bounds.left << "," << bounds.top << "," << bounds.width << ","<< bounds.height); 

			return false;
		}*/

		if((objects.size() < MAX_OBJECTS))
		{

			if(this->bounds.contains(pt)){
				LOGD("Pushed object to QuadTree#" << this);
				objects.push_back(obj);
			}
		}
		else
		{
			if( NE == nullptr) splitQuad();

			if(NE != nullptr && (NE->insert(obj, objBounds) || NW->insert(obj, objBounds) || SW->insert(obj, objBounds) || SE->insert(obj, objBounds)))
				return true;
		}

	};



	QuadTree<T>* getTree(sf::Vector2f)
	{
		QuadTree<T>* retTree = nullptr;

		// NorthEast intersection
		if(NE->bounds.contains(pt))
		{
			retTree = NE;
		}
		if(NW->bounds.contains(pt))
		{
			retTree = NW;
		}

		if(SE->bounds.contains(pt))
		{
			retTree = SE;
		}

		if(SW->bounds.contains(pt))
		{
			retTree = SW;
		}

		return retTree;

	};


	std::vector<T*> retrieve(sf::FloatRect ePos);



protected:
	int MAX_OBJECTS, MAX_LEVEL, level;
	sf::FloatRect bounds;
	std::vector<T*> objects;

	QuadTree<T>* NE;
	QuadTree<T>* NW;
	QuadTree<T>* SE;
	QuadTree<T>* SW;

};