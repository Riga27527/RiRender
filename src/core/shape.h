#pragma once

#include "common.h"

class Shape{
public:
	Shape(){}
	Shape(const Transform* obj2wor, const Transform* wor2obj)
	 : object2World(obj2wor), world2Object(wor2obj){}
	virtual ~Shape();

	virtual bool intersect() = 0;

	const Transform* object2World, *world2Object;
};