#include <lights/diffuse.h>
#include <core/shape.h>

RIGA_NAMESPACE_BEGIN

DiffuseAreaLight::DiffuseAreaLight(const Transform& light2world, const Spectrum& Le, int nSamples, 
		const std::shared_ptr<Shape>& shape, bool twoSided)
		: AreaLight(light2world, nSamples), 
		  Lemit(Le), 
		  shape(shape), 
		  twoSided(twoSided){

}

Spectrum DiffuseAreaLight::sample_Li(const Interaction& ref, const Point2f& u, 
						  Vec3f* wi, float *pdf, VisibilityTester* vis) const{
	Interaction pShape = shape->sample(ref, u, pdf);
	if(*pdf == 0.f || (pShape.p - ref.p).lengthSquared() == 0){
		*pdf = 0.f;
		return Spectrum(0.f);
	}
	*wi = Normalize(pShape.p - ref.p);
	*vis = VisibilityTester(ref, pShape);
	return L(pShape, -*wi);
}

float DiffuseAreaLight::pdf_Li(const Interaction& ref, const Vec3f& wi) const{
	return shape->pdf(ref, wi);
}

RIGA_NAMESPACE_END