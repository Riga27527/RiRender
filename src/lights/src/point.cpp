#include <lights/point.h>

RIGA_NAMESPACE_BEGIN

Spectrum PointLight::sample_Li(const Interaction& ref, const Point2f& sample, 
							  Vec3f* wi, float *pdf, VisibilityTester* vis) const{
	*wi = Normalize(pLight - ref.p);
	*vis = VisibilityTester(ref, Interaction(pLight, ref.time));
	*pdf = 1.f;
	return intensity / DistanceSquared(pLight, ref.p);
}

float PointLight::pdf_Li(const Interaction& ref, const Vec3f& wi) const{
	return 0;
}

RIGA_NAMESPACE_END