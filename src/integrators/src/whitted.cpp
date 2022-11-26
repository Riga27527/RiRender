#include <integrators/whitted.h>
#include <core/scene.h>
#include <core/light.h>

RIGA_NAMESPACE_BEGIN

Spectrum WhittedIntegrator::Li(const Ray& ray, const Scene& scene, Sampler& sampler, int depth) const{
	Spectrum L(0.);
	SurfaceInteraction inter;

	// if not hit anything, return background color
	if(!scene.intersect(ray, &inter)){
		return Spectrum(0.f);
	}

	Vec3f normal = Normalize(Vec3f(inter.shading.n));
	Vec3f wo = inter.wo;
	inter.computeScatteringFunctions(ray);

	L += inter.Le(wo);
	for(const auto &light : scene.lights){
		Vec3f wi;
		float pdf = 0.f;
		VisibilityTester vis;
		Spectrum L_light = light->sample_Li(inter, sampler.get2D(), &wi, &pdf, &vis);
		if(L_light.isBlack() || pdf == 0.f)
			continue;
		Spectrum f = inter.bsdf->f(wo, wi);
		if(!f.isBlack() && vis.unoccluded(scene))
			L += f * L_light * AbsDot(wi, normal) / pdf;
	}

	if(depth + 1 < maxDepth){
		Spectrum L_tmp = specularReflect(ray, inter, scene, sampler, depth);
		// if(L_tmp != Spectrum(0.f))
		// 	std::cout << L_tmp << std::endl;
		L += L_tmp;
		L += specularTransmit(ray, inter, scene, sampler, depth);
	}
	return L;
}

RIGA_NAMESPACE_END