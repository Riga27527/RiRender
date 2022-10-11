#include "mirror.h"
#include "spectrum.h"
#include "reflection.h"
#include "interaction.h"
#include "texture.h"

RIGA_NAMESPACE_BEGIN

void MirrorMaterial::computeScatteringFunctions(
	SurfaceInteraction* isect, TransportMode mode) const{
	isect->bsdf = std::make_unique<BSDF>(*isect);
	Spectrum r = Clamp(kr->evaluate(*isect), 0, Infinity);
	if(!r.isBlack()){
		isect->bsdf->add(new SpecularReflection(r, std::make_unique<FresnelNoOp>()));
	}
}

RIGA_NAMESPACE_END