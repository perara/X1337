#pragma once
#include "Object.h"

#include <map>
#include <vector>

class Factory{
public:
	Factory(int initialRequest);
	virtual void produceObjects(int quantity) = 0;
	virtual std::vector<Object*> requestBatch(int quantity) = 0;
	virtual Object* requestObject() = 0;

protected:
		std::map<std::string, std::vector<Object*>> objects;
};