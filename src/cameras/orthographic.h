#pragma once

#include "../core/camera.h"

RIGA_NAMESPACE_BEGIN

class OrthographicCamera : public ProjectiveCamera{
public:
	OrthographicCamera(const Transform& cam2wor, 
		const Bounds2f& screenWindow, const Vec2f& resolution)
		: ProjectiveCamera(cam2wor, Orthographic(0, 1), screenWindow, resolution){}
	float generateRay(const CameraSample& sample, Ray* ray) const;
};

OrthographicCamera* CreateOrthographicCamera(const Transform& cam2wor, const Vec2f& resolution);

RIGA_NAMESPACE_END