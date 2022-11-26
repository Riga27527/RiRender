#pragma once

#include <core/common.h>
#include <core/transform.h>

RIGA_NAMESPACE_BEGIN
enum class LightFlags : int {
	DeltaPosition = 1,
	DeltaDirection = 2,
	Area = 4,
	Infinite = 8
}; 

class Light{
public:
	virtual ~Light(){}
	Light(LightFlags flags, const Transform& light2world, int nSamples = 1);
	virtual Spectrum sample_Li(const Interaction& ref, const Point2f& sample, 
							  Vec3f* wi, float *pdf, VisibilityTester* vis) const = 0;
	virtual float pdf_Li(const Interaction& ref, const Vec3f& wi) const = 0;

	LightFlags flags;
	const int nSamples;

protected:
	const Transform light2world, world2light;
};

class AreaLight : public Light{
public:
	AreaLight(const Transform& light2world, int nSamples);
	virtual Spectrum L(const Interaction& isec, const Vec3f& w) const = 0;
};

class VisibilityTester{
public:
	VisibilityTester(){}
	VisibilityTester(const Interaction& p0, const Interaction& p1)
		: p0(p0), p1(p1){ }
	bool unoccluded(const Scene& scene) const;

private:
	Interaction p0, p1;
};
RIGA_NAMESPACE_END