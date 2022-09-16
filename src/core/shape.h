#pragma once

#include "common.h"
#include "transform.h"
#include "interaction.h"

RIGA_NAMESPACE_BEGIN

class Shape{
public:
	Shape(){}
	Shape(const Transform* obj2wor, const Transform* wor2obj, bool reverseOrient = false);
	virtual ~Shape();
	virtual Bounds3f objectBound() const = 0;
	virtual Bounds3f worldBound() const;
	virtual bool intersect(const Ray& ray, float *tHit, SurfaceInteraction* isect) const = 0;
	virtual bool intersectP(const Ray& ray) const{
		return 	intersect(ray, nullptr, nullptr);
	}
	virtual float Area() const = 0;


	const Transform* object2World, *world2Object;
	const bool reverseOrientation = false;
};

RIGA_NAMESPACE_END