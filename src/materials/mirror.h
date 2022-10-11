#pragma once

#include "common.h"
#include "material.h"

RIGA_NAMESPACE_BEGIN

class MirrorMaterial : public Material{
public:
	MirrorMaterial(const std::shared_ptr<Texture<Spectrum>> &r) : kr(r){}
	void computeScatteringFunctions(
		SurfaceInteraction* isect, TransportMode mode) const;

private:
	std::shared_ptr<Texture<Spectrum>> kr;
};

RIGA_NAMESPACE_END