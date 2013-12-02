#pragma once
#include <vector>
#include <map>
#include <SFML\Graphics.hpp>
#include "Log.h"
#include <vector>
#include <list>

template <class T>
class SliceEngine
{
private:
	sf::FloatRect bounds;
	int MAX_SLICE;
	sf::Vector2f SLICE_SIZE;

public:

	std::vector<std::list<T>> slices;

	// Bounds = window bounds
	// MAX_SLICE = num of slices
	SliceEngine(sf::FloatRect bounds, int MAX_SLICE):
		bounds(bounds),
		MAX_SLICE(MAX_SLICE),
		SLICE_SIZE( sf::Vector2f(bounds.width, bounds.height / MAX_SLICE))
	{
		slices.resize(MAX_SLICE + 1); // Set size to MAX_SLICE

	};



	void insert(T object, sf::FloatRect objBounds)
	{
		int index = (objBounds.top) / SLICE_SIZE.y;

		if(objBounds.left < 0 ||
			objBounds.top < 0 || 
			objBounds.left > bounds.width ||
			objBounds.top > bounds.height)
		{
			LOGD("Object ignored (Out of bounds)");
		}
		else
		{
			slices[index].push_back(object);
			LOGD("Inserted into region: " << index << "Size: " <<slices[index].size());
		};
	}

	std::vector<T> retrieve(sf::FloatRect objBounds)
	{
		int startIndex =  (objBounds.top) / SLICE_SIZE.y;
		int endIndex =  (objBounds.height + objBounds.top) / SLICE_SIZE.y;

		std::vector<T> result;
		for(int i = startIndex; i <= endIndex; i++)
		{
			for(int j = 0; j < slices[i].size(); j++)
			{
				T& obj = slices[i].front();
				result.push_back(obj);
				slices[i].pop_front();
				slices[i].push_back(obj);
			}
		}
		return result;
	}







};
