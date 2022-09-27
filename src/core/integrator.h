#pragma once

#include "common.h"
#include "sampler.h"

RIGA_NAMESPACE_BEGIN

class Integrator{
public:
	virtual ~Integrator();
	virtual void Render(const Scene& scene) = 0;
};

class SamplerIntegrator : public Integrator{
public:
	SamplerIntegrator()

private:
	std::shared_ptr<Sampler> sampler;
	
};

RIGA_NAMESPACE_END

