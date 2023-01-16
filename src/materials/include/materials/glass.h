#pragma once

#include <core/common.h>
#include <core/material.h>

RIGA_NAMESPACE_BEGIN

class GlassMaterial : public Material{
public:
    GlassMaterial(const std::shared_ptr<Texture<Spectrum>> &kr, 
                const std::shared_ptr<Texture<Spectrum>> &kt,
                const std::shared_ptr<Texture<float>> &eta)
                : kr(kr), kt(kt), index(eta){}
	void computeScatteringFunctions(
		SurfaceInteraction* isect, TransportMode mode) const;

private:
    std::shared_ptr<Texture<Spectrum>> kr;
    std::shared_ptr<Texture<Spectrum>> kt;
    std::shared_ptr<Texture<float>> index;
};

RIGA_NAMESPACE_END