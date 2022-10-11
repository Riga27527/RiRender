#include "integrator.h"
#include "spectrum.h"
#include "interaction.h"
#include "scene.h"
#include "light.h"

RIGA_NAMESPACE_BEGIN
void SamplerIntegrator::render(const Scene& scene){
	int m = 0;
	int width = camera->film->fullResolution.x, height = camera->film->fullResolution.y;
	std::vector<Spectrum> framebuffer(width * height);
	for(int i=0; i<height; ++i){
		for(int j=0; j<width; ++j){	
			int offset = i * width + j;
			std::unique_ptr<Sampler> pixel_sampler = sampler->clone(offset);
			Point2i pixel(j, i);
			pixel_sampler->startPixel(pixel);
			Spectrum L(0.f);
			do{
				CameraSample cs = pixel_sampler->getCameraSample(pixel);
				Ray r;
				camera->generateRay(cs, &r);
				L += Li(r, scene, *pixel_sampler);
			}while(pixel_sampler->startNextSample());
			framebuffer[m++] = L / (float)pixel_sampler->samplesPerPixel;
		}
	}
	camera->film->write2PPM(framebuffer);
}

Spectrum SamplerIntegrator::specularReflect(const Ray& ray, const SurfaceInteraction& isec, 
	const Scene& scene, Sampler& sampler, int depth) const{
	Vec3f wo = isec.wo, wi;
	float pdf;

	BxDFType type = BxDFType(BSDF_REFLECTION | BSDF_SPECULAR);
	Spectrum f = isec.bsdf->sample_f(wo, &wi, sampler.get2D(), &pdf, type);

	const Normal3f& ns = isec.shading.n;
	if(pdf > 0.f && !f.isBlack() && AbsDot(wi, ns) != 0.f){
		Ray rd = isec.spawnRay(wi);
		return f * Li(rd, scene, sampler, depth + 1) * AbsDot(wi, ns) / pdf;
	}else
		return Spectrum(0.f);
}

Spectrum SamplerIntegrator::specularTransmit(const Ray& ray, const SurfaceInteraction& isec, 
	const Scene& scene, Sampler& sampler, int depth) const{
	Vec3f wo = isec.wo, wi;
	float pdf;

	BxDFType type = BxDFType(BSDF_TRANSMISSION | BSDF_SPECULAR);
	Spectrum f = isec.bsdf->sample_f(wo, &wi, sampler.get2D(), &pdf, type);

	const Normal3f& ns = isec.shading.n;
	if(pdf > 0.f && !f.isBlack() && AbsDot(wi, ns) != 0.f){
		Ray rd = isec.spawnRay(wi);
		return f * Li(rd, scene, sampler, depth + 1) * AbsDot(wi, ns) / pdf;
	}else
		return Spectrum(0.f);
}



RIGA_NAMESPACE_END