#include "light.h"
#include "scene.h"

RIGA_NAMESPACE_BEGIN

Light::Light(LightFlags flags, const Transform& light2world, int nSamples)
	: flags(flags), nSamples(std::max(1, nSamples)), 
	  light2world(light2world), world2light(Inverse(light2world)){ }

AreaLight::AreaLight(const Transform& light2world, int nSamples)
	: Light(LightFlags::Area, light2world, nSamples){ }

bool VisibilityTester::unoccluded(const Scene& scene) const{
	return !scene.intersectP(p0.spawnRayTo(p1));
}

RIGA_NAMESPACE_END