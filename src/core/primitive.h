#pragma once

#include "common.h"
#include "geometry.h"
#include "shape.h"
#include <vector>

RIGA_NAMESPACE_BEGIN

class Primitive{
public:
	virtual ~Primitive(){}
	virtual Bounds3f worldBound() const = 0;
	virtual bool intersect(const Ray& ray, SurfaceInteraction *) const = 0;
	virtual bool intersectP(const Ray& ray) const = 0;
	virtual void computeScatteringFunctions() const = 0;

};


class GeometricPrimitive : public Primitive{
public:
	GeometricPrimitive(const std::shared_ptr<Shape>& shape) 
		: shape(shape){};
	Bounds3f worldBound() const;
	bool intersect(const Ray& ray, SurfaceInteraction* isect) const;
	bool intersectP(const Ray& ray) const;
	void computeScatteringFunctions() const;

private:
	std::shared_ptr<Shape> shape;
};

class Aggregate{
public:
	Aggregate(std::vector<std::shared_ptr<Primitive>> p) : primitives(std::move(p)){}
	virtual ~Aggregate(){}
	virtual Bounds3f worldBound() const = 0;
	virtual bool intersect(const Ray& ray, SurfaceInteraction *) const = 0;
	virtual bool intersectP(const Ray& ray) const = 0;

protected:
	std::vector<std::shared_ptr<Primitive>> primitives;
};

RIGA_NAMESPACE_END