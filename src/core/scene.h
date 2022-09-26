#pragma once

#include "common.h"
#include "primitive.h"
#include "geometry.h"

RIGA_NAMESPACE_BEGIN
class Scene{
public:
	Scene(std::unique_ptr<Aggregate> aggregate)
		: agg(std::move(aggregate)){
			sceneBound = agg->worldBound();
		}
	const Bounds3f& worldBound() const{
		return sceneBound;
	}
	bool intersect(const Ray& ray, SurfaceInteraction* isect) const;
	bool intersectP(const Ray& ray) const;

private:
	Bounds3f sceneBound;
	std::unique_ptr<Aggregate> agg;
};

RIGA_NAMESPACE_END