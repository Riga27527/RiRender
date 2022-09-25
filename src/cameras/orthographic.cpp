#include "orthographic.h"

RIGA_NAMESPACE_BEGIN

float OrthographicCamera::generateRay(const CameraSample& sample, Ray* ray) const{
	Point3f pInRaster = Point3f(sample.samplePoints.x, sample.samplePoints.y, 0);
	Point3f pInCamera = raster2camera(pInRaster);

	// In camera space, always cast ray to z axis
	*ray = Ray(pInCamera, Vec3f(0.f, 0.f, 1.f));
	*ray = camera2world(*ray);
	return 1;
}

OrthographicCamera* CreateOrthographicCamera(const Transform& cam2wor, const Vec2f& resolution){
	Bounds2f screenWindow;
	float frameRatio = resolution.x / resolution.y;
	if(frameRatio > 1.f){
		screenWindow.pMin.x = -frameRatio;
		screenWindow.pMax.x = frameRatio;
		screenWindow.pMin.y = -1;
		screenWindow.pMax.y = 1;
	}else{
		screenWindow.pMin.x = -1;
		screenWindow.pMax.x = 1;
		screenWindow.pMin.y = -1.f / frameRatio;
		screenWindow.pMax.y = 1.f / frameRatio;		
	}
	float screenScale = 2.f;
	{
		screenWindow.pMin *= screenScale;
		screenWindow.pMax *= screenScale;
	}
	return new OrthographicCamera(cam2wor, screenWindow, resolution);
}

RIGA_NAMESPACE_END