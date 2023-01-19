#include <integrators/path.h>
#include <core/scene.h>
#include <core/sampler.h>
#include <core/light.h>

RIGA_NAMESPACE_BEGIN

Spectrum PathIntegrator::Li(const Ray& ray, const Scene& scene, Sampler& sampler, int depth = 0) const{
    Spectrum L(0.f), throughput(1.f);
    float pdf_light = 0.f, pdf_bsdf = 0.f;
    for(unsigned bounce = 0; bounce < maxDepth; ++bounce){
        SurfaceInteraction isect;
        bool hasIntersection = scene.intersect(ray, &isect);
        
        if(bounce == 0){
            if(hasIntersection)
                L += throughput * isect.Le(-ray.dir);
            // TODO : add infinite area Light
        }

        if(!hasIntersection)
            break;        
        
        isect.computeScatteringFunctions(ray);
        
        // direct
        for(const auto &light : scene.lights){
            Vec3f wi;
            pdf_light = 0.f;
            VisibilityTester vis;
            Spectrum L_light = light->sample_Li(isect, sampler.get2D(), &wi, &pdf_light, &vis);
            if(L_light.isBlack() || pdf_light == 0.f)
                continue;
            if(vis.unoccluded(scene)){
                pdf_bsdf = isect.bsdf->pdf(isect.wo, wi);
                if(pdf_bsdf != 0.f && pdf_light != 0.f){
                    float mis_pdf = pdf_light / (pdf_bsdf + pdf_light);
                    Spectrum L_direct = throughput * L_light;
                    L += L_direct;
                }
            }
        }

        // indirect
        Vec3f wo = isect.wo, wi;
        Vec3f normal = Normalize(Vec3f(isect.shading.n));
        pdf_bsdf = 0.f;
        Spectrum f = isect.bsdf->sample_f(wo, &wi, sampler.get2D(), &pdf_bsdf);
        if(f.isBlack() || pdf_bsdf == 0.f)
            break;
        throughput *= f * AbsDot(wi, normal);
    }
}

RIGA_NAMESPACE_END