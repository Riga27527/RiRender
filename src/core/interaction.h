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
	void computeScatteringFunctions();

	const Shape* shape = nullptr;
	const Primitive* primitive = nullptr;
};

RIGA_NAMESPACE_END