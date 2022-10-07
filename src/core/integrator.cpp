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
			Spectrum Li(0.f);
			do{
				CameraSample cs = pixel_sampler->getCameraSample(pixel);
				// CameraSample cs;
				// cs.samplePoints = Point2f(j + random_float(), i + random_float());
				Ray r;
				camera->generateRay(cs, &r);
				SurfaceInteraction inter;
				if(scene.intersect(r, &inter)){
					inter.computeScatteringFunctions(r);
					Vec3f normal = Normalize(Vec3f(inter.shading.n));
					Vec3f wo = inter.wo;
					Li += inter.Le(wo);
					for(const auto &light : scene.lights){
						Vec3f wi;
						float pdf = 0.f;
						VisibilityTester vis;
						Spectrum L_light = light->sample_Li(inter, pixel_sampler->get2D(), &wi, &pdf, &vis);
						if(L_light.isBlack() || pdf == 0.f)
							continue;
						Spectrum f = inter.bsdf->f(wo, wi);
						if(!f.isBlack() && vis.unoccluded(scene))
							Li += f * L_light * AbsDot(wi, normal) / pdf;
					}
				}
			}while(pixel_sampler->startNextSample());
			framebuffer[m++] = Li / (float)pixel_sampler->samplesPerPixel;
		}
	}
	camera->film->write2PPM(framebuffer);
}

RIGA_NAMESPACE_END