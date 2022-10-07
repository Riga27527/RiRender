#include "interaction.h"
#include "primitive.h"
#include "light.h"

RIGA_NAMESPACE_BEGIN

SurfaceInteraction::SurfaceInteraction(const Point3f& p, const Normal3f& n, 
	const Vec3f& wo, float t, const Point2f& UV, const Shape* sh)
	: Interaction(p, n, wo, t), uv(UV), shape(sh){
		shading.n = n;
	}

void SurfaceInteraction::setShadingInfo(const Normal3f& shadingNormal){
	shading.n = shadingNormal;
}

void SurfaceInteraction::computeScatteringFunctions(const Ray& ray, TransportMode mode){
	primitive->computeScatteringFunctions(this, mode);
}

Spectrum SurfaceInteraction::Le(const Vec3f& w) const{
	AreaLight* light = primitive->getAreaLight();
	return light ? light->L(*this, w) : Spectrum(0.f);
}
RIGA_NAMESPACE_END