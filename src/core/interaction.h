#pragma once

#include "common.h"
#include "geometry.h"

RIGA_NAMESPACE_BEGIN

struct Interaction{
	Interaction(){}
	Interaction(const Point3f& p, float t) : p(p), time(t){}
	Interaction(const Point3f& p, const Vec3f& wo, float t)
	: p(p), wo(wo), time(t){}
	Interaction(const Point3f& p, const Normal3f& n, const Vec3f& wo, float t)
	: p(p), n(n), wo(wo), time(t){}
	Ray spawnRay(const Vec3f& d2) const{
		// Point3f o = p + 0.1f * Vec3f(n);
		return Ray(p, d2, RayEpsilon, Infinity, time);
	}
	Ray spawnRayTo(const Point3f& p2) const{
		// Point3f o = p + 0.1f * Vec3f(n);
		Vec3f dir = p2 - p;
		return Ray(p, dir, RayEpsilon, 1.f-RayEpsilon, time);
	}
	Ray spawnRayTo(const Interaction& inter) const{
		// Point3f o = p + 0.1f * Vec3f(n);
		Vec3f dir = inter.p - p;
		return Ray(p, dir, RayEpsilon, 1.f-RayEpsilon, time);		
	}
	float time = 0.f;
	Point3f p;
	Vec3f wo;
	Normal3f n;
};

class SurfaceInteraction : public Interaction{
public:
	SurfaceInteraction(){}
	SurfaceInteraction(const Point3f& p, const Normal3f& n, const Vec3f& wo, float t, 
		const Point2f& UV, const Shape* sh);
	void computeScatteringFunctions();
	void setShadingInfo(const Normal3f& shadingNormal);

	const Shape* shape = nullptr;
	const Primitive* primitive = nullptr;
	Point2f uv;
	struct{
		Normal3f n;
	}shading;
};

RIGA_NAMESPACE_END