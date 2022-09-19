#include "primitive.h"

RIGA_NAMESPACE_BEGIN


Bounds3f GeometricPrimitive::worldBound() const{
	return shape->worldBound();
}

bool GeometricPrimitive::intersect(const Ray& ray, SurfaceInteraction* isect) const{
	float tHit;
	if(!shape->intersect(ray, &tHit, isect))
		return false;
	ray.tMax = tHit;
	isect->primitive = this;
	return true;
}

bool GeometricPrimitive::intersectP(const Ray& ray) const{
	return shape->intersectP(ray);
}

void GeometricPrimitive::computeScatteringFunctions() const{

}

RIGA_NAMESPACE_END