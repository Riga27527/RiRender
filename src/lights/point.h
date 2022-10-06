#pragma once

#include "common.h"
#include "light.h"

RIGA_NAMESPACE_BEGIN
class PointLight : public Light{
public:
	PointLight(const Transform& light2world, const Spectrum& I)
	: Light(LightFlags::DeltaPosition, light2world), 
	  pLight(light2world(Point3f(0.f))),
	  intensity(I){ }

	Spectrum sample_Li(const Interaction& ref, const Point2f& sample, 
							  Vec3f* wi, float *pdf, VisibilityTester* vis) const;
	float pdf_Li(const Interaction& ref, const Vec3f& wi) const;

private:
	const Point3f pLight;
	const Spectrum intensity;
};

RIGA_NAMESPACE_END