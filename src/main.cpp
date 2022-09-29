#include <iostream>
#include <vector>
#include <memory>
#include <type_traits>
#include "geometry.h"
#include "transform.h"
#include "common.h"
#include "sphere.h"
#include "triangle.h"
#include "bvh.h"
#include "spectrum.h"
#include "scene.h"
#include "perspective.h"
#include "orthographic.h"
#include "halton.h"
#include "integrator.h"
#include "film.h"

using namespace riga;

void OBJ_loader_BVH_test(int width, int height){

	// get Obj Mesh
	Transform Tri_obj2wor, Tri_wor2obj;
	Tri_obj2wor = Tri_obj2wor * Translate(Vec3f(0.f, -1.5f, 0.f)) * RotateY(180) * Scale(Vec3f(15.f, 15.f, 15.f));
	std::string obj_path("/home/cs18/Rider/scenes/obj/bunny.obj"); 
	std::vector<std::shared_ptr<Shape>> tri_mesh =
		CreateOBJMesh(&Tri_obj2wor, &Tri_wor2obj, false, obj_path);

	// get Floor Mesh
	int nTris = 2;
	int nVers = 4;
	const float yPos = -2.f; 
	Point3f p[] = {Point3f(-6.f, yPos, -6.f), Point3f(-6.f, yPos, 6.f), 
				Point3f(6.f, yPos, 6.f), Point3f(6.f, yPos, -6.f)};
	const int vertexIndex[6] = {0, 1, 2, 2, 3, 0};
	Transform identity;
	std::vector<std::shared_ptr<Shape>>	floor_mesh = CreateTriangleMesh(&identity, &identity, false, 
		nTris, nVers, vertexIndex, p, nullptr, nullptr, nullptr);

	// get Aggragate and Scenes
	std::vector<std::shared_ptr<Primitive>> prims;
	for(size_t i=0; i<tri_mesh.size(); ++i)
		prims.push_back(std::make_shared<GeometricPrimitive>(tri_mesh[i]));
	for(size_t i=0; i<floor_mesh.size(); ++i)
		prims.push_back(std::make_shared<GeometricPrimitive>(floor_mesh[i]));	
	std::unique_ptr<Aggregate> agg = std::make_unique<BVH>(prims);
	std::unique_ptr<Scene> scene = std::make_unique<Scene>(std::move(agg));

	// get camera
	Point3f eye(0.f, 0.f, -3.f), look(0.f, 0.f, 0.f);
	Vec3f up(0.f, 1.f, 0.f);
	Transform lookAt = LookAt(eye, look, up);
	Transform cam2wor = Inverse(lookAt);
	float fov = 90.f;
	std::unique_ptr<Film> film = std::make_unique<Film>(Point2i(width, height), "random20.ppm");
	std::shared_ptr<Camera> cam(CreatePerspectiveCamera(cam2wor, fov, film.get()));

	// get sampler and integrator
	Bounds2i imageBound(Point2i(0, 0), Point2i(width, height));
	std::shared_ptr<Sampler> halsamp = std::make_shared<HaltonSampler>(20, imageBound, false);
	std::shared_ptr<Integrator> integrator = std::make_shared<SamplerIntegrator>(cam, halsamp);
	integrator->render(*scene);
}

int main(int argc, char const *argv[])
{
	std::cout << "Hello Rider!" << std::endl;
	OBJ_loader_BVH_test(800, 800);
	return 0;
}