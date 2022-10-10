#pragma once

#include "common.h"
#include "sampler.h"
#include "camera.h"
RIGA_NAMESPACE_BEGIN

class Integrator{
public:
	virtual ~Integrator(){}
	virtual void render(const Scene& scene) = 0;
};

class SamplerIntegrator : public Integrator{
public:
	SamplerIntegrator(std::shared_ptr<const Camera> cam, std::shared_ptr<Sampler> samp)
	: camera(cam), sampler(samp){}
	
	Spectrum specularReflec(const Ray& ray, const SurfaceInteraction& isec, 
		const Scene& scene, Sampler& sampler, int depth) const;
	Spectrum specularTransmit(const Ray& ray, const SurfaceInteraction& isec, 
		const Scene& scene, Sampler& sampler, int depth) const;

	virtual Spectrum Li(const Ray& ray, const Scene& scene, Sampler& sampler, int depth = 0) const = 0;
	void render(const Scene& scene);

protected:
	std::shared_ptr<const Camera> camera;
private:
	std::shared_ptr<Sampler> sampler;

};

RIGA_NAMESPACE_END

