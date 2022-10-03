#include "reflection.h"

RIGA_NAMESPACE_BEGIN

Spectrum BSDF::f(const Vec3f& wo_world, const Vec3f& wi_world, 
	BxDFType flags) const{
	Vec3f wo = world2Local(wo_world), wi = world2Local(wi_world);
	Spectrum ret(0);
	if(wo.z == 0)
		return ret;
	bool reflect = Dot(wo, ng) * Dot(wi, ng) > 0;
	for(size_t i=0; i<nBxDFs; ++i){
		if(bxdfs[i]->matchesFlags(flags) && 
			((reflect && (bxdfs[i]->type & BSDF_REFLECTION)) ||
			 (!reflect && (bxdfs[i]->type & BSDF_TRANSMISSION))))
			ret += bxdfs[i]->f(wo, wi);
	}
	return ret;
}

Spectrum BSDF::sample_f(const Vec3f& wo_world, Vec3f* wi_world, const Point2f& u, 
	float *pdf, BxDFType type = BSDF_ALL) const{
	int matchingComps = getNumComponents(type);
	if(matchingComps == 0){
		*pdf = 0;
		return Spectrum(0);
	}
	// sample a bxdf to get wi
	int comp = std::min((int)std::floor(u[0] * matchingComps), matchingComps - 1);
	BxDF *bxdf = nullptr;
	int count = comp;
	for(size_t i=0; i<nBxDFs; ++i)
		if(bxdfs[i]->matchesFlags(type) && count-- == 0){
			bxdf = bxdfs[i];
			break;
		}
	// remap a sample
	Point2f uRemapped(u[0] * matchingComps - comp, u[1]);
	Vec3f wi, wo = world2Local(wo_world);
	*pdf = 0.f;
	if(wo.z == 0)
		return Spectrum(0);
	Spectrum f = bxdf->sample_f(wo, wi, uRemapped, pdf);
	if(*pdf == 0)
		return Spectrum(0);
	*wi_world = local2World(wi);

	if(!(bxdf->type & BSDF_SPECULAR) && matchingComps > 1){
		for(size_t i=0; i<nBxDFs; ++i)
	}

}

float BSDF::pdf(const Vec3f& wo, const Vec3f& wi, BxDFType flags = BSDF_ALL) const;


RIGA_NAMESPACE_END