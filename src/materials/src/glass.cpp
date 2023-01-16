#include <materials/glass.h>
#include <core/interaction.h>
#include <core/texture.h>
#include <core/reflection.h>

RIGA_NAMESPACE_BEGIN

void GlassMaterial::computeScatteringFunctions(
    SurfaceInteraction* isect, TransportMode mode) const{
    isect->bsdf = std::make_unique<BSDF>(*isect);
    Spectrum R = Clamp(kr->evaluate(*isect), 0, Infinity);
    Spectrum T = Clamp(kr->evaluate(*isect), 0, Infinity);
    float eta = Clamp(index->evaluate(*isect), 0, Infinity);

	if(!R.isBlack()){
		isect->bsdf->add(new SpecularReflection(R, std::make_unique<FresnelDielectric>(1.f, eta)));
	}
    if(!T.isBlack()){
		isect->bsdf->add(new SpecularTransmission(T, 1.f, eta, mode));
	}
}

RIGA_NAMESPACE_END