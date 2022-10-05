#include "matte.h"
#include "spectrum.h"
#include "reflection.h"
#include "interaction.h"
#include "texture.h"

RIGA_NAMESPACE_BEGIN

void MatteMaterial::computeScatteringFunctions(
	SurfaceInteraction* isect, TransportMode mode) const{
	isect->bsdf = std::make_unique<BSDF>(*isect);
	Spectrum r = Clamp(kd->evaluate(*isect), 0, Infinity);
	float sig = Clamp(sigma->evaluate(*isect), 0, 90);
	if(!r.isBlack()){
		if(sig == 0)
			isect->bsdf->add(new LambertianReflection(r));
	}
}

RIGA_NAMESPACE_END