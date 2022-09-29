#pragma once

#include "shape.h"

RIGA_NAMESPACE_BEGIN

class Sphere : public Shape{
public:
	Sphere(const Transform* obj2wor, const Transform* wor2obj, bool reverseOrient,
			float r) : Shape(obj2wor, wor2obj, reverseOrient), radius(r){}
	Bounds3f objectBound() const;
	bool intersect(const Ray& ray, float *tHit, SurfaceInteraction* isect) const;
	bool intersectP(const Ray& ray) const;
	float Area() const;

	
	float radius;
};

RIGA_NAMESPACE_END