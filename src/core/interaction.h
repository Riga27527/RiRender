#pragma once

#include "common.h"
#include "shape.h"
#include "primitive.h"

RIGA_NAMESPACE_BEGIN

struct Interaction{
	Interaction(){}
	Interaction(const Point3f& p, float t) : p(p), time(t){}
	Interaction(const Point3f& p, const Vec3f& wo, float t)
	: p(p), wo(wo), time(t){}
	Interaction(const Point3f& p, const Normal3f& n, const Vec3f& wo, float t)
	: p(p), n(n), wo(wo), time(t){}
	
	
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