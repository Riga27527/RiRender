#pragma once

#include <core/common.h>
#include <core/integrator.h>

RIGA_NAMESPACE_BEGIN
class PathIntegrator : public SamplerIntegrator
{
public:
    PathIntegrator(int maxDepth, std::shared_ptr<const Camera> cam, std::shared_ptr<Sampler> samp)
        : SamplerIntegrator(cam, samp), maxDepth(maxDepth){}
    Spectrum Li(const Ray& ray, const Scene& scene, Sampler& sampler, int depth = 0) const;

private:
    const int maxDepth;
};

RIGA_NAMESPACE_END