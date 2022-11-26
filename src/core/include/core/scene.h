#pragma once

#include <core/common.h>
#include <core/primitive.h>
#include <core/geometry.h>

RIGA_NAMESPACE_BEGIN
class Scene{
public:
	Scene(std::unique_ptr<Aggregate> aggregate, const std::vector<std::shared_ptr<Light>>& lights)
		: lights(lights), agg(std::move(aggregate)){
			sceneBound = agg->worldBound();
		}
	const Bounds3f& worldBound() const{
		return sceneBound;
	}
	bool intersect(const Ray& ray, SurfaceInteraction* isect) const;
	bool intersectP(const Ray& ray) const;

	std::vector<std::shared_ptr<Light>> lights;

private:
	Bounds3f sceneBound;
	std::unique_ptr<Aggregate> agg;
};

RIGA_NAMESPACE_END