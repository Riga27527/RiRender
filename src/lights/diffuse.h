#pragma once

#include "common.h"
#include "light.h"

RIGA_NAMESPACE_BEGIN

class DiffuseAreaLight : public AreaLight{
public:
	DiffuseAreaLight(const Transform& light2world, const Spectrum& Le, int nSamples, 
		const std::shared_ptr<Shape>& shape, bool twoSided = false);

	Spectrum L(const Interaction& isec, const Vec3f& w) const{
		return (twoSided || Dot(isec.n, w) > 0) ? Lemit : Spectrum(0.f);
	}
	Spectrum sample_Li(const Interaction& ref, const Point2f& sample, 
							  Vec3f* wi, float *pdf, VisibilityTester* vis) const;
	float pdf_Li(const Interaction& ref, const Vec3f& wi) const;


private:
	const Spectrum Lemit;
	std::shared_ptr<Shape> shape;
	const bool twoSided;
};

RIGA_NAMESPACE_END