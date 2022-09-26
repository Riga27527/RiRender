#include "scene.h"

RIGA_NAMESPACE_BEGIN

bool Scene::intersect(const Ray& ray, SurfaceInteraction* isect) const{
	return agg->intersect(ray, isect);
}

bool Scene::intersectP(const Ray& ray) const{
	return agg->intersectP(ray);
}

RIGA_NAMESPACE_END