#include <iostream>
#include <vector>
#include <memory>
#include <type_traits>
#include <core/geometry.h>
#include <core/transform.h>
#include <core/common.h>
#include <shapes/sphere.h>
#include <shapes/triangle.h>
#include <accelerators/bvh.h>
#include <core/spectrum.h>
#include <core/scene.h>
#include <cameras/perspective.h>
#include <cameras/orthographic.h>
#include <samplers/halton.h>
#include <core/integrator.h>
#include <core/film.h>
#include <textures/constant.h>
#include <materials/matte.h>
#include <lights/point.h>
#include <lights/diffuse.h>
#include <integrators/whitted.h>
#include <materials/mirror.h>

using namespace riga;

void OBJ_loader_BVH_test(int width, int height){

	// Materials
	float bunny_color[3] = {0.2f, 0.5f, 0.4f};
	std::shared_ptr<Texture<Spectrum>> bunny_kd 
		= std::make_shared<ConstantTexture<Spectrum>>(Spectrum::fromRGB(bunny_color));
	std::shared_ptr<Material> bunny_mat = std::make_shared<MatteMaterial>(bunny_kd);

	std::shared_ptr<Texture<Spectrum>> floor_kd 
		= std::make_shared<ConstantTexture<Spectrum>>(Spectrum(0.5f));
	std::shared_ptr<Material> floor_mat = std::make_shared<MatteMaterial>(floor_kd);

	std::shared_ptr<Texture<Spectrum>> mirror_kd 
		= std::make_shared<ConstantTexture<Spectrum>>(Spectrum(0.7f));
	std::shared_ptr<Material> mirror_mat = std::make_shared<MirrorMaterial>(mirror_kd);

	float red_color[3] = {0.9f, 0.05f, 0.05f};
	std::shared_ptr<Texture<Spectrum>> red_kd 
		= std::make_shared<ConstantTexture<Spectrum>>(Spectrum::fromRGB(red_color));
	std::shared_ptr<Material> red_mat = std::make_shared<MatteMaterial>(red_kd);

	float green_color[3] = {0.05f, 0.9f, 0.05f};
	std::shared_ptr<Texture<Spectrum>> green_kd 
		= std::make_shared<ConstantTexture<Spectrum>>(Spectrum::fromRGB(green_color));
	std::shared_ptr<Material> green_mat = std::make_shared<MatteMaterial>(green_kd);

	float light_color[3] = {0.98f, 0.98f, 0.98f};
	std::shared_ptr<Texture<Spectrum>> light_kd 
		= std::make_shared<ConstantTexture<Spectrum>>(Spectrum::fromRGB(light_color));
	std::shared_ptr<Material> light_mat = std::make_shared<MatteMaterial>(light_kd);	

	// Primitives
	std::vector<std::shared_ptr<Primitive>> prims;

	// get Obj Mesh
	Transform Tri_obj2wor, Tri_wor2obj;
	Tri_obj2wor = Tri_obj2wor * Translate(Vec3f(0.f, -1.5f, 1.2f)) * RotateY(180) * Scale(Vec3f(10.f, 10.f, 10.f));
	Tri_wor2obj = Inverse(Tri_obj2wor);
	std::string obj_path("/home/riga/Rider/scenes/obj/bunny.obj"); 
	std::vector<std::shared_ptr<Shape>> tri_mesh =
		CreateOBJMesh(&Tri_obj2wor, &Tri_wor2obj, false, obj_path);
	for(size_t i=0; i<tri_mesh.size(); ++i)
		prims.push_back(std::make_shared<GeometricPrimitive>(tri_mesh[i], bunny_mat));

	// get Floor Mesh
	int nTris = 2;
	int nVers = 4;
	const float yPos = -2.f; 
	Point3f p[] = {Point3f(-6.f, yPos, -6.f), Point3f(-6.f, yPos, 6.f), 
				Point3f(6.f, yPos, 6.f), Point3f(6.f, yPos, -6.f)};
	Normal3f nor[] = {Normal3f(0.f, 1.f, 0.f), Normal3f(0.f, 1.f, 0.f), 
				Normal3f(0.f, 1.f, 0.f), Normal3f(0.f, 1.f, 0.f)};	
	const int vertexIndex[6] = {0, 1, 2, 2, 3, 0};
	Transform identity;
	std::vector<std::shared_ptr<Shape>>	floor_mesh = CreateTriangleVector(&identity, &identity, false, 
		nTris, nVers, vertexIndex, p, nor, nullptr, nullptr);
	for(size_t i=0; i<floor_mesh.size(); ++i)
		prims.push_back(std::make_shared<GeometricPrimitive>(floor_mesh[i], mirror_mat));

	// get left Wall Mesh
	int nTris_left = 2;
	int nVers_left = 4;
	const float xPos = -3.f; 
	Point3f p_left[] = {Point3f(xPos, 6.f, 6.f), Point3f(xPos, -6.f, 6.f), 
				Point3f(xPos, -6.f, -6.f), Point3f(xPos, 6.f, -6.f)};
	const int vertexIndex_left[6] = {0, 1, 2, 2, 3, 0};
	std::vector<std::shared_ptr<Shape>>	left_wall_mesh = CreateTriangleVector(&identity, &identity, true, 
		nTris_left, nVers_left, vertexIndex_left, p_left, nullptr, nullptr, nullptr);
	for(size_t i=0; i<left_wall_mesh.size(); ++i)
		prims.push_back(std::make_shared<GeometricPrimitive>(left_wall_mesh[i], red_mat));

	// get Right Wall Mesh
	Transform left2right_obj2wor, left2right_wor2obj;
	left2right_obj2wor = Translate(Vec3f(6.f, 0.f, 0.f)) * left2right_obj2wor;
	left2right_wor2obj = Inverse(left2right_obj2wor);
	std::vector<std::shared_ptr<Shape>>	right_wall_mesh = CreateTriangleVector(
		&left2right_obj2wor, &left2right_wor2obj, false, nTris_left, nVers_left, vertexIndex_left, p_left, 
		nullptr, nullptr, nullptr);
	for(size_t i=0; i<right_wall_mesh.size(); ++i)
		prims.push_back(std::make_shared<GeometricPrimitive>(right_wall_mesh[i], green_mat));	

	// get front Wall Mesh
	int nTris_front = 2;
	int nVers_front = 4;
	const float zPos = 3.f; 
	Point3f p_front[] = {Point3f(-6.f, 6.f, zPos), Point3f(-6.f, -6.f, zPos), 
				Point3f(6.f, -6.f, zPos), Point3f(6.f, 6.f, zPos)};
	const int vertexIndex_front[6] = {0, 1, 2, 2, 3, 0};
	std::vector<std::shared_ptr<Shape>>	front_wall_mesh = CreateTriangleVector(&identity, &identity, false, 
		nTris_front, nVers_front, vertexIndex_front, p_front, nullptr, nullptr, nullptr);
	for(size_t i=0; i<front_wall_mesh.size(); ++i)
		prims.push_back(std::make_shared<GeometricPrimitive>(front_wall_mesh[i], floor_mat));

	// get Ceil Mesh
	Transform bottom2top_obj2wor, bottom2top_wor2obj;
	bottom2top_obj2wor = Translate(Vec3f(0.f, 6.f, 0.f)) * bottom2top_obj2wor;
	bottom2top_wor2obj = Inverse(bottom2top_obj2wor);
	std::vector<std::shared_ptr<Shape>>	cell_mesh = CreateTriangleVector(
		&bottom2top_obj2wor, &bottom2top_wor2obj, true, 
		nTris, nVers, vertexIndex, p, nor, nullptr, nullptr);
	for(size_t i=0; i<cell_mesh.size(); ++i)
		prims.push_back(std::make_shared<GeometricPrimitive>(cell_mesh[i], floor_mat));

	std::vector<std::shared_ptr<Light>> lights;
	// Point Lights
	// Transform light2world_1, light2world_2;
	// light2world_1 = Translate(Vec3f(-2.f, 4.f, 3.f)) * light2world_1;
	// light2world_2 = Translate(Vec3f(2.f, 4.f, -3.f)) * light2world_2;
	// std::shared_ptr<Light> plight1 = std::make_shared<PointLight>(light2world_1, Spectrum(50.f));
	// std::shared_ptr<Light> plight2 = std::make_shared<PointLight>(light2world_2, Spectrum(50.f));
	// lights.push_back(plight1);

	// Area Lights	
	int nTrisLight = 2;
	int nVersLight = 4;
	const float yPosLight = 2.8f; 
	Point3f pLight[] = {Point3f(-0.8f, yPosLight, 1.8f), Point3f(-0.8f, yPosLight, 0.2f), 
				Point3f(0.8f, yPosLight, 0.2f), Point3f(0.8f, yPosLight, 1.8f)};
	const int vtxIdxLight[6] = {0, 3, 2, 2, 1, 0};
	std::vector<std::shared_ptr<Shape>>	areaLight_mesh = CreateTriangleVector(&identity, &identity, false, 
		nTris, nVers, vertexIndex, pLight, nullptr, nullptr, nullptr);
	for(const auto& tri : areaLight_mesh){
		std::shared_ptr<AreaLight> areaLight = std::make_shared<DiffuseAreaLight>(identity, Spectrum(30.f), 5, tri, false);
		lights.push_back(areaLight);
		auto lit_prim = std::make_shared<GeometricPrimitive>(tri, light_mat, areaLight);
		prims.push_back(lit_prim);	
	}
		
	// Aggragate and Scenes
	std::unique_ptr<Aggregate> agg = std::make_unique<BVH>(prims);
	std::unique_ptr<Scene> scene = std::make_unique<Scene>(std::move(agg), lights);

	// get camera
	Point3f eye(0.f, 0.f, -3.f), look(0.f, 0.f, 0.f);
	Vec3f up(0.f, 1.f, 0.f);
	Transform lookAt = LookAt(eye, look, up);
	Transform cam2wor = Inverse(lookAt);
	float fov = 90.f;
	std::unique_ptr<Film> film = std::make_unique<Film>(Point2i(width, height), "whitted_test_mirror.ppm");
	std::shared_ptr<Camera> cam(CreatePerspectiveCamera(cam2wor, fov, film.get()));

	// get sampler and integrator
	Bounds2i imageBound(Point2i(0, 0), Point2i(width, height));
	std::shared_ptr<Sampler> halsamp = std::make_shared<HaltonSampler>(64, imageBound, false);
	std::shared_ptr<Integrator> integrator = std::make_shared<WhittedIntegrator>(5, cam, halsamp);
	integrator->render(*scene);
}

int main(int argc, char const *argv[])
{
	// **********************LEFT HAND COORDINATE**********************************
	std::cout << "Hello Rider!" << std::endl;
	OBJ_loader_BVH_test(800, 800);
	return 0;
}