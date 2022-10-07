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

void GeometricPrimitive::computeScatteringFunctions(SurfaceInteraction* isect, TransportMode mode) const{
	if(material)
		material->computeScatteringFunctions(isect, mode);
	if(Dot(isect->n, isect->shading.n) < 0)
		throw("Shaing normal and geometry normal are not in a same hemiSphere!");
}

AreaLight* GeometricPrimitive::getAreaLight() const{
	return areaLight.get();
}

Material* GeometricPrimitive::getMaterial() const{
	return material.get();
}


RIGA_NAMESPACE_END