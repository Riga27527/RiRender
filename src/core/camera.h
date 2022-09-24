#pragma once

#include "geometry.h"
#include "transform.h"

RIGA_NAMESPACE_BEGIN

class Camera{
public:
	Camera(const Transform& cam2wor) : camera2world(cam2wor){}
	virtual ~Camera(){}
	virtual float generateRay(const CameraSample& sample, Ray *ray) const = 0;
	
	Transform camera2world;
};

struct CameraSample{
	Point2f samplePoints;
};

class ProjectiveCamera : public Camera{
public:
	ProjectiveCamera(const Transform& cam2wor, const Transform& cam2screen, 
		const Bounds2f& screenWindow, const Vec2f& resolution)
		: Camera(cam2wor), camera2screen(cam2screen){
			screen2raster = 
			Scale(resolution.x, resolution.y, 1) *
			Scale(1.f / (screenWindow.diagnoal().x), - 1.f / (screenWindow.diagnoal().y), 1) *
			Translate(Vec3f(-screenWindow.pMin.x, -screenWindow.pMax.y, 0));
			raster2screen = Inverse(screen2raster);
			raster2camera = Inverse(camera2screen) * raster2screen;
		}
protected:
	Transform camera2screen, screen2raster;
	Transform raster2screen, raster2camera;
};
RIGA_NAMESPACE_END