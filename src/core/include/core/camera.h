#pragma once

#include <core/geometry.h>
#include <core/transform.h>
#include <core/film.h>

RIGA_NAMESPACE_BEGIN

class Camera{
public:
	Camera(const Transform& cam2wor, Film* film) 
	: camera2world(cam2wor), film(film){}
	
	virtual ~Camera(){}
	virtual float generateRay(const CameraSample& sample, Ray *ray) const = 0;
	
	Transform camera2world;
	Film* film;
};

struct CameraSample{
	Point2f samplePoints;
};

class ProjectiveCamera : public Camera{
public:
	ProjectiveCamera(const Transform& cam2wor, const Transform& cam2screen, 
		const Bounds2f& screenWindow, Film* film)
		: Camera(cam2wor, film), camera2screen(cam2screen){
			screen2raster = 
			Scale(film->fullResolution.x, film->fullResolution.y, 1) *
			Scale(1.f / (screenWindow.pMax.x - screenWindow.pMin.x), 
				- 1.f / (screenWindow.pMax.y - screenWindow.pMin.y), 1) *
			Translate(Vec3f(-screenWindow.pMin.x, -screenWindow.pMax.y, 0));
			raster2screen = Inverse(screen2raster);
			raster2camera = Inverse(camera2screen) * raster2screen;

		}
protected:
	Transform camera2screen, screen2raster;
	Transform raster2screen, raster2camera;
};
RIGA_NAMESPACE_END