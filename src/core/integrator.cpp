#include "integrator.h"
#include "spectrum.h"
#include "interaction.h"
#include "scene.h"

RIGA_NAMESPACE_BEGIN
void SamplerIntegrator::render(const Scene& scene){
	int m = 0;
	Point3f light(-2.f, 4.f, -3.f);
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
					Vec3f lightDir = Normalize(light - inter.p);
					Vec3f normal = Normalize(Vec3f(inter.shading.n));
					if(!scene.intersectP(inter.spawnRayTo(light))){
						// diff
						float diff = std::max(Dot(normal, lightDir), 0.f);

						//spec
						Vec3f halfVec = Normalize(lightDir + Normalize(-r.dir));
						float spec = std::pow(std::max(Dot(halfVec, normal), 0.f), 32.f);
						Li += Spectrum(0.2 + 0.4 * diff + 0.8 * spec);
					}
				}
			}while(pixel_sampler->startNextSample());
			framebuffer[m++] = Li / (float)pixel_sampler->samplesPerPixel;
		}
	}
	camera->film->write2PPM(framebuffer);
}

RIGA_NAMESPACE_END