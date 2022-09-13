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
	GeometricPrimitive(const std::shared_ptr<Shape>& shape);
	virtual Bounds3f worldBound() const;
	virtual bool intersect(const Ray& ray, SurfaceInteraction *) const;
	virtual bool intersectP(const Ray& ray) const;
	virtual void computeScatteringFunctions() const;

private:
	std::shared_ptr<Shape> shape;
};

class Aggregate{
	virtual ~Aggregate(){}
	virtual Bounds3f worldBound() const = 0;
	virtual bool intersect(const Ray& ray, SurfaceInteraction *) const = 0;
	virtual bool intersectP(const Ray& ray) const = 0;

protected:
	std::vector<std::shared_ptr<Primitive>> primitives;
};

RIGA_NAMESPACE_END