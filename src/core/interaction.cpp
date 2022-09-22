#include "interaction.h"

RIGA_NAMESPACE_BEGIN

SurfaceInteraction::SurfaceInteraction(const Point3f& p, const Normal3f& n, 
	const Vec3f& wo, float t, const Point2f& UV, const Shape* sh)
	: Interaction(p, n, wo, t), uv(UV), shape(sh){
		shading.n = n;
	}

void SurfaceInteraction::setShadingInfo(const Normal3f& shadingNormal){
	shading.n = shadingNormal;
}

RIGA_NAMESPACE_END