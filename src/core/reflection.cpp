#include "reflection.h"
#include "sampling.h"
#include "interaction.h"

RIGA_NAMESPACE_BEGIN

// BxDF Utility Functions
Float FrDielectric(Float cosThetaI, Float etaI, Float etaT) {
    cosThetaI = Clamp(cosThetaI, -1, 1);
    // Potentially swap indices of refraction
    bool entering = cosThetaI > 0.f;
    if (!entering) {
        std::swap(etaI, etaT);
        cosThetaI = std::abs(cosThetaI);
    }

    // Compute _cosThetaT_ using Snell's law
    Float sinThetaI = std::sqrt(std::max((Float)0, 1 - cosThetaI * cosThetaI));
    Float sinThetaT = etaI / etaT * sinThetaI;

    // Handle total internal reflection
    if (sinThetaT >= 1) return 1;
    Float cosThetaT = std::sqrt(std::max((Float)0, 1 - sinThetaT * sinThetaT));
    Float Rparl = ((etaT * cosThetaI) - (etaI * cosThetaT)) /
                  ((etaT * cosThetaI) + (etaI * cosThetaT));
    Float Rperp = ((etaI * cosThetaI) - (etaT * cosThetaT)) /
                  ((etaI * cosThetaI) + (etaT * cosThetaT));
    return (Rparl * Rparl + Rperp * Rperp) / 2;
}

// https://seblagarde.wordpress.com/2013/04/29/memo-on-fresnel-equations/
Spectrum FrConductor(Float cosThetaI, const Spectrum &etai,
                     const Spectrum &etat, const Spectrum &k) {
    cosThetaI = Clamp(cosThetaI, -1, 1);
    Spectrum eta = etat / etai;
    Spectrum etak = k / etai;

    Float cosThetaI2 = cosThetaI * cosThetaI;
    Float sinThetaI2 = 1. - cosThetaI2;
    Spectrum eta2 = eta * eta;
    Spectrum etak2 = etak * etak;

    Spectrum t0 = eta2 - etak2 - sinThetaI2;
    Spectrum a2plusb2 = Sqrt(t0 * t0 + 4 * eta2 * etak2);
    Spectrum t1 = a2plusb2 + cosThetaI2;
    Spectrum a = Sqrt(0.5f * (a2plusb2 + t0));
    Spectrum t2 = (Float)2 * cosThetaI * a;
    Spectrum Rs = (t1 - t2) / (t1 + t2);

    Spectrum t3 = cosThetaI2 * a2plusb2 + sinThetaI2 * sinThetaI2;
    Spectrum t4 = t2 * sinThetaI2;
    Spectrum Rp = Rs * (t3 - t4) / (t3 + t4);

    return 0.5 * (Rp + Rs);
}

BSDF::BSDF(const SurfaceInteraction& isec, float eta)
	: eta(eta), ns(isec.shading.n), ng(isec.n){
			CoordinateSystem(ns, ss, ts);
}

int BSDF::getNumComponents(BxDFType flags) const{
	int num = 0;
	for(size_t i=0; i<nBxDFs; ++i)
		if(bxdfs[i]->matchesFlags(flags))
			++num;
	return num;
}

BSDF::~BSDF(){
	for(size_t i=0; i<nBxDFs; ++i)
		bxdfs[i]->~BxDF();
}

Spectrum BSDF::f(const Vec3f& wo_world, const Vec3f& wi_world, 
	BxDFType flags) const{
	Vec3f wo = world2Local(wo_world), wi = world2Local(wi_world);
	Spectrum ret(0);
	if(wo.z == 0)
		return ret;
	bool reflect = Dot(wo_world, ng) * Dot(wi_world, ng) > 0;
	for(size_t i=0; i<nBxDFs; ++i){
		if(bxdfs[i]->matchesFlags(flags) && 
			((reflect && (bxdfs[i]->type & BSDF_REFLECTION)) ||
			 (!reflect && (bxdfs[i]->type & BSDF_TRANSMISSION))))
			ret += bxdfs[i]->f(wo, wi);
	}
	return ret;
}

Spectrum BSDF::sample_f(const Vec3f& wo_world, Vec3f* wi_world, const Point2f& u, 
	float *pdf, BxDFType type) const{
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
	Spectrum f = bxdf->sample_f(wo, &wi, uRemapped, pdf);
	if(*pdf == 0)
		return Spectrum(0);
	*wi_world = local2World(wi);

	// get avg pdf
	if(!(bxdf->type & BSDF_SPECULAR) && matchingComps > 1){
		for(size_t i=0; i<nBxDFs; ++i)
			if(bxdfs[i] != bxdf && bxdfs[i]->matchesFlags(type))
				*pdf += bxdfs[i]->pdf(wo, wi);
	}
	if(matchingComps > 1)
		*pdf /= matchingComps;

	// get total f
	if(!(bxdf->type & BSDF_SPECULAR)){
		f = Spectrum(0.f);
		bool reflect = Dot(wo_world, ng) * Dot(*wi_world, ng) > 0;
		for(size_t i=0; i<nBxDFs; ++i){
			if(bxdfs[i]->matchesFlags(type) && 
				((reflect && (bxdfs[i]->type & BSDF_REFLECTION)) ||
				 (!reflect && (bxdfs[i]->type & BSDF_TRANSMISSION))))
				f += bxdfs[i]->f(wo, wi);
		} 
	}
	return f;
}

float BSDF::pdf(const Vec3f& wo_world, const Vec3f& wi_world, BxDFType flags) const{
	if(nBxDFs == 0)
		return 0.f;
	Vec3f wo = world2Local(wo_world), wi = world2Local(wi_world);
	if(wo.z == 0)
		return 0.f;
	float pdf = 0.f;
	int matchingComps = 0;
	for(size_t i=0; i<nBxDFs; ++i){
		if(bxdfs[i]->matchesFlags(flags)){
			++matchingComps;
			pdf += bxdfs[i]->pdf(wo, wi);
		}
	}
	return (matchingComps > 0 ? pdf / matchingComps : 0.f);
}

Spectrum BxDF::sample_f(const Vec3f& wo, Vec3f* wi, const Point2f& sample, float* pdf_) const{
	*wi = square2CosineHemishpere(sample);
	if(wo.z < 0)
		wi->z *= -1;
	*pdf_ = pdf(wo, *wi);
	return f(wo, *wi);
}

float BxDF::pdf(const Vec3f& wo, const Vec3f& wi) const{
	return SameHemiSphere(wo, wi) ? AbsCosTheta(wi) * INV_PI : 0;
}

Spectrum LambertianReflection::f(const Vec3f& wo, const Vec3f& wi) const{
	return R * INV_PI;
}

RIGA_NAMESPACE_END