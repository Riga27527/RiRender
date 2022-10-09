#pragma once

#include "common.h"
#include "geometry.h"
#include "spectrum.h"

RIGA_NAMESPACE_BEGIN

enum BxDFType{
	BSDF_REFLECTION = 1 << 0,
	BSDF_TRANSMISSION = 1 << 1,
	BSDF_DIFFUSE = 1 << 2,
	BSDF_GLOSSY = 1 << 3,
	BSDF_SPECULAR = 1 << 4,
	BSDF_ALL = BSDF_DIFFUSE | BSDF_GLOSSY | BSDF_SPECULAR | BSDF_REFLECTION | BSDF_TRANSMISSION,
};

float FrDielectric(float cosThetaI, float etaI, float etaT);

Spectrum FrConductor(float cosThetaI, const Spectrum& etaI, const Spectrum& etaT, const Spectrum& k);

class BSDF{
public:
	BSDF(const SurfaceInteraction& isec, float eta = 1.f); 
	void add(BxDF *b){
		if(nBxDFs >= MaxBxDFs)
			throw("The number of BxDF in BSDF is too many!");
		bxdfs[nBxDFs++] = b;
	}
	int getNumComponents(BxDFType flags = BSDF_ALL) const;

	Vec3f world2Local(const Vec3f& v) const{
		return Vec3f(Dot(ss, v), Dot(ts, v), Dot(ns, v));
	}
	Vec3f local2World(const Vec3f& v) const{
		return Vec3f(ss.x * v.x + ts.x * v.y + ns.x * v.z,
					 ss.y * v.x + ts.y * v.y + ns.y * v.z,
					 ss.z * v.x + ts.z * v.y + ns.z * v.z);
	}
	Spectrum f(const Vec3f& wo_world, const Vec3f& wi_world, 
		BxDFType flags = BSDF_ALL) const;

	Spectrum sample_f(const Vec3f& wo_world, Vec3f* wi_world, const Point2f& u, 
		float *pdf, BxDFType type = BSDF_ALL) const;
	
	float pdf(const Vec3f& wo_world, const Vec3f& wi_world, BxDFType flags = BSDF_ALL) const;

	~BSDF();

	// friend std::ostream& operator<<(std::ostream& os, const BSDF& bsdf){
	// 	os << "[ BSDF eta: " << eta << " ,nBxDFs: " << nBxDFs;
	// 	for(size_t i=0; i<nBxDFs; ++i)
	// 		os << "\n bxdfs[" << i << "]: " << *bxdfs[i];
	// 	os << "]";
	// 	return os;
	// }
	const float eta;
private:
	static constexpr int MaxBxDFs = 8;
	const Normal3f ns, ng;
	Vec3f ss, ts;
	int nBxDFs = 0;
	BxDF *bxdfs[MaxBxDFs];
};

class BxDF{
public:
	virtual ~BxDF(){}
	BxDF(BxDFType type) : type(type){}
	bool matchesFlags(BxDFType t) const{
		return (type & t) == type;
	}
	virtual Spectrum f(const Vec3f& wo, const Vec3f& wi) const = 0;
	virtual Spectrum sample_f(const Vec3f& wo, Vec3f* wi, const Point2f& sample, float* pdf) const;
	virtual float pdf(const Vec3f& wo, const Vec3f& wi) const;

	const BxDFType type;
};

class LambertianReflection : public BxDF {
public:
	LambertianReflection(const Spectrum& R)
		: BxDF(BxDFType(BSDF_REFLECTION | BSDF_DIFFUSE)), R(R){}
	Spectrum f(const Vec3f& wo, const Vec3f& wi) const;

private:
	const Spectrum R;
};


class Fresnel{
public:
	virtual ~Fresnel();
	virtual Spectrum evaluate(float cosThetaI) const = 0;
	virtual std::string toString() const = 0;
};

class FresnelConductor : public Fresnel{
public:
	Spectrum evaluate(float cosThetaI) const;
	FresnelConductor(const Spectrum& etaI, const Spectrum& etaT, const Spectrum& k)
		: etaI(etaI), etaT(etaT), k(k){}

private:
	Spectrum etaI, etaT, k;
};

class FresnelDielectric : public Fresnel{
public:
	Spectrum evaluate(float cosThetaI) const;
	FresnelDielectric(float etaI, float etaT)
		: etaI(etaI), etaT(etaT){}
private:
	float etaI, etaT;
};

class FresnelNoOp : public Fresnel{
public:
	Spectrum evaluate(float) const {
		return Spectrum(1.f);
	}
};

class SpecularReflection : public BxDF{
public:

private:
	const Spectrum R;
	const Fresnel* fresnel;
};

RIGA_NAMESPACE_END