#pragma once

#include <core/common.h>
#include <core/transform.h>
#include <core/interaction.h>

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
	virtual float area() const = 0;

	virtual Interaction sample(const Point2f& u, float *pdf) const = 0;
	virtual float pdf(const Interaction& ) const{
		return 1.f / area();
	}

	virtual Interaction sample(const Interaction& ref, const Point2f& u, float *pdf) const;
	virtual float pdf(const Interaction& ref, const Vec3f& wi) const;

	const Transform* object2World, *world2Object;
	const bool reverseOrientation = false;
};

RIGA_NAMESPACE_END