#include <materials/matte.h>
#include <core/spectrum.h>
#include <core/reflection.h>
#include <core/interaction.h>
#include <core/texture.h>

RIGA_NAMESPACE_BEGIN

void MatteMaterial::computeScatteringFunctions(
	SurfaceInteraction* isect, TransportMode mode) const{
	isect->bsdf = std::make_unique<BSDF>(*isect);
	Spectrum r = Clamp(kd->evaluate(*isect), 0, Infinity);
	if(!r.isBlack()){
		isect->bsdf->add(new LambertianReflection(r));
	}
}

RIGA_NAMESPACE_END