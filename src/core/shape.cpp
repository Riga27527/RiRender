#include "shape.h"

RIGA_NAMESPACE_BEGIN

static long long nShapesCreated = 0;

Shape::Shape(const Transform* obj2wor, const Transform* wor2obj, bool reverseOrient)
			: object2World(obj2wor), world2Object(wor2obj), reverseOrientation(reverseOrient){
				++nShapesCreated;
			}

Shape::~Shape(){}

Bounds3f Shape::worldBound() const{
	return (*object2World)(this->objectBound());
}

Interaction Shape::sample(const Interaction& ref, const Point2f& u, float *pdf) const{
	Interaction isec = sample(u, pdf);
	Vec3f wi = isec.p - ref.p;
	float length2 = wi.lengthSquared();
	if(length2 == 0)
		*pdf = 0.f;
	else{
		wi = Normalize(wi);
		// Area pdf -> Solid Angle pdf
		*pdf *= (length2 / AbsDot(isec.n, -wi));
		if(std::isinf(*pdf))
			*pdf = 0.f;
	}
	return isec;
}

float Shape::pdf(const Interaction& ref, const Vec3f& wi) const{
	Ray ray = ref.spawnRay(wi);
	float tHit;
	SurfaceInteraction isecLight;
	if(!this->intersect(ray, &tHit, &isecLight))
		return 0.f;

	float pdf = DistanceSquared(ref.p, isecLight.p) / (AbsDot(isecLight.n, -wi) * area());
	if(std::isinf(pdf))
		pdf = 0.f;	
	return pdf;
}


RIGA_NAMESPACE_END