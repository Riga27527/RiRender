#pragma once

#include "../core/camera.h"

RIGA_NAMESPACE_BEGIN

class PerspectiveCamera : public ProjectiveCamera{
public:
	PerspectiveCamera(
		const Transform& cam2wor, const Bounds2f& screenWindow, 
		float fov, const Vec2f& resolution) 
		: ProjectiveCamera(cam2wor, Perspective(fov, 1e-2f, 1000.f), screenWindow, resolution){}
	float generateRay(const CameraSample& sample, Ray* ray) const;
};

PerspectiveCamera* CreatePerspectiveCamera(const Transform& cam2wor, float fov, const Vec2f& resolution);

RIGA_NAMESPACE_END