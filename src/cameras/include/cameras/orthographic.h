#pragma once

#include <core/camera.h>

RIGA_NAMESPACE_BEGIN

class OrthographicCamera : public ProjectiveCamera{
public:
	OrthographicCamera(const Transform& cam2wor, 
		const Bounds2f& screenWindow, Film* film)
		: ProjectiveCamera(cam2wor, Orthographic(0, 1), screenWindow, film){}
	float generateRay(const CameraSample& sample, Ray* ray) const;
};

OrthographicCamera* CreateOrthographicCamera(const Transform& cam2wor, Film* film);

RIGA_NAMESPACE_END