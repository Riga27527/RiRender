#include "sphere.h"

RIGA_NAMESPACE_BEGIN

Bounds3f Sphere::objectBound() const{
	return Bounds3f(Point3f(Point3f(-radius)), Point3f(radius));
}

bool Sphere::intersect(const Ray& ray, float *tHit, SurfaceInteraction* isect) const{
	// simply from rto
	// Intersect in Object space
	Ray r = (*world2Object)(ray);
	Vec3f oc = r.o - Point3f(0.f);
	float a = r.dir.lengthSquared();
	float half_b = Dot(oc, r.dir);
	float c = oc.lengthSquared() - radius * radius;
	float discriminant = half_b * half_b - a * c;
	if(discriminant < 0)
		return false;

	// find the smallest positive root
	float dis_sqrt = std::sqrt(discriminant);
	float root = (-half_b - dis_sqrt) / a;
	if(root < r.tMin || root > r.tMax){
		root = (-half_b + dis_sqrt) / a;
		if(root < r.tMin || root > r.tMax)
			return false;
	}
	Point3f pHit = r.at(root);
	Normal3f normal = Normalize(Normal3f(pHit - Point3f(0.f)));
	float phi = std::atan2(pHit.y, pHit.x);
	if(phi < 0)
		phi += PI * 2;
	float theta = std::acos(pHit.z / radius);
	float u = phi / (2 * PI);
	float v = theta / PI;
	*tHit = root;
	*isect = (*object2World)(SurfaceInteraction(pHit, normal, -r.dir, r.time, Point2f(u, v), this));
	return true;
}

bool Sphere::intersectP(const Ray& ray) const{
	
	// Intersect in Object space
	Ray r = (*world2Object)(ray);
	Vec3f oc = r.o - Point3f(0.f);
	float a = r.dir.lengthSquared();
	float half_b = Dot(oc, r.dir);
	float c = oc.lengthSquared() - radius * radius;
	float discriminant = half_b * half_b - a * c;
	if(discriminant < 0)
		return false;

	// find the smallest positive root
	float dis_sqrt = std::sqrt(discriminant);
	float root = (-half_b - dis_sqrt) / a;
	if(root < r.tMin || root > r.tMax){
		root = (-half_b + dis_sqrt) / a;
		if(root < r.tMin || root > r.tMax)
			return false;
	}
	return true;	
}

float Sphere::Area() const{
	return 4.f * PI * radius * radius;
}


RIGA_NAMESPACE_END