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
#include "constant.h"
#include "matte.h"
#include "point.h"

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

	// Lights
	Transform light2world_1, light2world_2;
	light2world_1 = Translate(Vec3f(-2.f, 4.f, -3.f)) * light2world_1;
	light2world_2 = Translate(Vec3f(2.f, 4.f, -3.f)) * light2world_2;
	std::shared_ptr<Light> plight1 = std::make_shared<PointLight>(light2world_1, Spectrum(50.f));
	std::shared_ptr<Light> plight2 = std::make_shared<PointLight>(light2world_2, Spectrum(50.f));
	std::vector<std::shared_ptr<Light>> lights;
	lights.push_back(plight1);
	// lights.push_back(plight2);

	// Materials
	std::shared_ptr<Texture<Spectrum>> kd = std::make_shared<ConstantTexture<Spectrum>>(Spectrum(0.6f));
	std::shared_ptr<Material> mat = std::make_shared<MatteMaterial>(kd);

	// Primitives
	std::vector<std::shared_ptr<Primitive>> prims;
	for(size_t i=0; i<tri_mesh.size(); ++i)
		prims.push_back(std::make_shared<GeometricPrimitive>(tri_mesh[i], mat));
	for(size_t i=0; i<floor_mesh.size(); ++i)
		prims.push_back(std::make_shared<GeometricPrimitive>(floor_mesh[i], mat));	
	
	// Aggragate and Scenes
	std::unique_ptr<Aggregate> agg = std::make_unique<BVH>(prims);
	std::unique_ptr<Scene> scene = std::make_unique<Scene>(std::move(agg), lights);

	// get camera
	Point3f eye(0.f, 0.f, -3.f), look(0.f, 0.f, 0.f);
	Vec3f up(0.f, 1.f, 0.f);
	Transform lookAt = LookAt(eye, look, up);
	Transform cam2wor = Inverse(lookAt);
	float fov = 90.f;
	std::unique_ptr<Film> film = std::make_unique<Film>(Point2i(width, height), "Halton_1pointLight.ppm");
	std::shared_ptr<Camera> cam(CreatePerspectiveCamera(cam2wor, fov, film.get()));

	// get sampler and integrator
	Bounds2i imageBound(Point2i(0, 0), Point2i(width, height));
	std::shared_ptr<Sampler> halsamp = std::make_shared<HaltonSampler>(2, imageBound, false);
	std::shared_ptr<Integrator> integrator = std::make_shared<SamplerIntegrator>(cam, halsamp);
	integrator->render(*scene);
}

int main(int argc, char const *argv[])
{
	std::cout << "Hello Rider!" << std::endl;
	OBJ_loader_BVH_test(800, 800);
	return 0;
}