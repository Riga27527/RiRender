#pragma once

#include "common.h"
#include "material.h"

RIGA_NAMESPACE_BEGIN

class MatteMaterial : public Material{
public:
	MatteMaterial(const std::shared_ptr<Texture<Spectrum>>& kd,
		const std::shared_ptr<Texture<float>>& sigma = nullptr) : kd(kd), sigma(sigma){}
	void computeScatteringFunctions(
		SurfaceInteraction* isect, TransportMode mode) const;

private:
	std::shared_ptr<Texture<Spectrum>> kd;
	std::shared_ptr<Texture<float>> sigma;	
};

RIGA_NAMESPACE_END