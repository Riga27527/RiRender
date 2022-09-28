#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <type_traits>
#include "core/geometry.h"
#include "core/transform.h"
#include "core/common.h"
#include "shapes/sphere.h"
#include "shapes/triangle.h"
#include "accelerators/bvh.h"
#include "core/spectrum.h"
#include "core/scene.h"
#include "cameras/perspective.h"
#include "cameras/orthographic.h"
#include "./samplers/halton.h"

using namespace riga;

void render_ppm(int width, int height){
	// Transform sphereT_obj2wor, sphereT_wor2obj;
	// sphereT_obj2wor = sphereT_obj2wor * Translate(Vec3f(1.0f, 1.f, 0.f));
	// sphereT_wor2obj = Inverse(sphereT_obj2wor);
	// std::shared_ptr<Shape> sphere = std::make_shared<Sphere>(&sphereT_obj2wor, &sphereT_wor2obj, false, 1.f);

	Transform Tri_obj2wor, Tri_wor2obj;
	// Tri_obj2wor = Tri_obj2wor * Translate(Vec3f(1.0f, 1.f, 0.f));
	int tri_num = 2, point_num = 6;
	const Point3f mesh_p[] = {
		Point3f(-1.f, 1.f, 0.f), Point3f(0.f, 1.f, 0.f), Point3f(1.f, 1.f, 0.f),
		Point3f(-1.f, -1.f, 0.f), Point3f(0.f, -1.f, 0.f), Point3f(1.f, -1.f, 0.f)
	};
	const int mesh_vInds[] = {0, 3, 1, 2, 4, 5};

	std::vector<std::shared_ptr<Shape>> tri_mesh = CreateTriangleMesh(
	&Tri_obj2wor, &Tri_wor2obj, false, tri_num, point_num, mesh_vInds, mesh_p, 
	nullptr, nullptr, nullptr, nullptr);

	std::vector<Vec3f> framebuffer(width * height);
	Camera* cam;
	Point3f eye(0.f, 0.f, -2.f), look(0.f, 0.f, 0.f);
	Vec3f up(0.f, 1.f, 0.f);
	Transform lookAt = LookAt(eye, look, up);
	Transform cam2wor = Inverse(lookAt);
	float fov = 90.f;
	cam = CreateOrthographicCamera(cam2wor, Vec2f(width, height));

	// Point3f lower_let_corner(-2.0f, -2.0f, 2.0f);
	// Point3f ray_o(0.f, 0.f, 4.f);
	// Vec3f horizontal(4.f, 0.f, 0.f);
	// Vec3f vertical(0.f, 4.f, 0.f);
	int m = 0;
	// std::cout << tri_mesh.size() << std::endl;
	for(int i=0; i<height; ++i){
		for(int j=0; j<width; ++j){	
			CameraSample cs;
			cs.samplePoints = Point2f(j + random_float(), i + random_float());
			Ray r;
			cam->generateRay(cs, &r);
			
			framebuffer[m] = Vec3f(1.f, 1.f, 1.f);
			for(size_t k=0; k<tri_mesh.size(); ++k){
				if(tri_mesh[k]->intersectP(r)){
					framebuffer[m] = Vec3f(1.f, 0.f, 0.f);
					break;
				}
			}
			++m;	
			// if(sphere->intersectP(r))
			// 	framebuffer[m++] = Vec3f(1.f, 0.f, 0.f);
			// else
			// 	framebuffer[m++] = Vec3f(1.f, 1.f, 1.f);
		}
	}


    FILE* fp = fopen("binary_tri.ppm", "wb");
    (void)fprintf(fp, "P6\n%d %d\n255\n", width, height);
    for (auto i = 0; i < height * width; ++i) {
        static unsigned char color[3];
        color[0] = (unsigned char)(255 * Clamp(0, 1, framebuffer[i].x));
        color[1] = (unsigned char)(255 * Clamp(0, 1, framebuffer[i].y));
        color[2] = (unsigned char)(255 * Clamp(0, 1, framebuffer[i].z));
        fwrite(color, 1, 3, fp);
    }
    fclose(fp); 
}


void OBJ_loader_BVH_test(int width, int height){
	// std::cout << std::is_same<RGBSpecturm, Spectrum>::value << std::endl;
	Transform Tri_obj2wor, Tri_wor2obj;
	Tri_obj2wor = Tri_obj2wor * Translate(Vec3f(0.f, -1.5f, 0.f)) * RotateY(180) * Scale(Vec3f(15.f, 15.f, 15.f));

	Vec3f light(1.0, 1.0, 1.0);
	std::string obj_path("/home/cs18/Rider/scenes/obj/bunny.obj"); 
	std::vector<std::shared_ptr<Shape>> tri_mesh =
		CreateOBJMesh(&Tri_obj2wor, &Tri_wor2obj, false, obj_path);

	std::vector<std::shared_ptr<Primitive>> prims;
	for(size_t i=0; i<tri_mesh.size(); ++i)
		prims.push_back(std::make_shared<GeometricPrimitive>(tri_mesh[i]));
	std::unique_ptr<Aggregate> agg = std::make_unique<BVH>(prims);
	std::unique_ptr<Scene> scene = std::make_unique<Scene>(std::move(agg));

	std::vector<Spectrum> framebuffer(width * height);
	Camera* cam;
	Point3f eye(0.f, 0.f, -3.f), look(0.f, 0.f, 0.f);
	Vec3f up(0.f, 1.f, 0.f);
	Transform lookAt = LookAt(eye, look, up);
	Transform cam2wor = Inverse(lookAt);
	float fov = 90.f;
	cam = CreateOrthographicCamera(cam2wor, Vec2f(width, height));

	int m = 0;
	// std::cout << tri_mesh.size() << std::endl;
	for(int i=0; i<height; ++i){
		for(int j=0; j<width; ++j){	
			CameraSample cs;
			cs.samplePoints = Point2f(j + random_float(), i + random_float());
			Ray r;
			cam->generateRay(cs, &r);
			SurfaceInteraction inter;
			Spectrum Li;
			if(scene->intersect(r, &inter)){
				Vec3f normal = Vec3f(inter.shading.n);
				Li[1] = std::abs(Dot(light, normal));
			}
			framebuffer[m++] = Li;
		}
	}

    FILE* fp = fopen("binary_ortho.ppm", "wb");
    (void)fprintf(fp, "P6\n%d %d\n255\n", width, height);
    for (auto i = 0; i < height * width; ++i) {
        static unsigned char color[3];
        color[0] = (unsigned char)(255 * Clamp(0, 1, framebuffer[i][0]));
        color[1] = (unsigned char)(255 * Clamp(0, 1, framebuffer[i][1]));
        color[2] = (unsigned char)(255 * Clamp(0, 1, framebuffer[i][2]));
        fwrite(color, 1, 3, fp);
    }
    fclose(fp); 
}

int main(int argc, char const *argv[])
{
	// std::cout << "Hello Rider!" << std::endl;
	// const size_t n = 100000;
	
	// TICK_TBB(for)
	// float m = 0;
	// for(size_t i=0; i < n; ++i)
	// 	m += i * 24 + 10;
	// TOCK_TBB(for)
	// std::cout << m << std::endl;

	// Point3<float> p1{2.0f, 4.0f, 9.0f}, p2;
	// Vec3<float> v1{3.0f, 4.0f, 5.0f}, v2;

	// p2 = (Point3<float>)v1;
	// v2 = (Vec3<float>)p1;

	// std::cout << "p2: " << p2 << std::endl;
	// std::cout << "v2: " << v2 << std::endl;

	// std::cout << v1.normalized() << std::endl;
	// std::cout << v1.Length() << std::endl;

	// Vec3<float> v3{5.0f, 6.0f, 7.0f};
	// Normal3<float> v4{-3.0f, -4.0f, -5.0f};
	// Vec2<float> v5{3.0, 4.0};

	// std::cout << Normalize(v3) << std::endl;
	// std::cout << v3.normalized() << std::endl;
	// std::cout << Normalize(v4) << std::endl;
	// std::cout << v4.normalized() << std::endl;
	// std::cout << v5.length() << std::endl;
	// std::cout << Normalize(v5) << std::endl;
	// std::cout << v5.normalized() << std::endl;

	// std::cout << Permute(v3, 2, 1, 0) << std::endl;
	// std::cout << Permute(p1, 1, 2, 0) << std::endl;
	// std::cout << Permute(v5, 1, 0) << std::endl;

	// std::cout << v4.dot(v3) << std::endl;
	// v4.faceForward(v3);
	// std::cout << v4 << std::endl;

	// Mat4x4f mat = Mat4x4f::identity();
	// mat[1][1] = 2.f;
	// // std::cout << mat[1][1] << std::endl;	

	// Transform trans(Mat4x4f::identity());
	// std::cout << trans.hasScale() << std::endl;

	// Transform trans2(mat);
	// std::cout << trans2.hasScale() << std::endl;
	// trans2.m[1][1] = 1.f;
	// trans2.m[0][2] = 3.f;
	// std::cout << trans2.hasScale() << std::endl;

	// auto t = LookAt(Point3f(5.f, 0.f, 0.f), Point3f(5.f, 0.f, -10.f), Vec3f(0.f, 1.0f, 0.f));
	// std::cout << t << std::endl;

	// Bounds3<float> b1(Point3f(0.f, -1.f, 0.f), Point3f(7.f, 8.f, 9.f)), b2(Point3f(-2.f, 1.f, 1.f), Point3f(10.f, 6.f, 3.f));
	// Bounds2<float> b3(Point2f(0.f, 1.f), Point2f(7.f, 8.f)), b4(Point2f(-2.f, 1.f), Point2f(10.f, 3.f));
	// Point3f p1(3.f, 2.f, 11.f);
	// Point2f p2(9.f, 8.f);

	// std::cout << Union(b1, b2) << std::endl;
	// std::cout << Union(b1, p1) << std::endl;
	// std::cout << Union(b3, b4) << std::endl;
	// std::cout << Union(b3, p2) << std::endl;
	// std::cout << Union(p1, p2) << std::endl;

	// Bounds3<float> b(Point3f(1.f, 1.f, 1.f), Point3f(5.f, 5.f, 5.f));
	// Ray r(Point3f(0.f, 0.f, 0.f), Vec3f(0.2f, 0.2f, 0.2f), 1);
	// bool neg[3] = {0, 0, 0};
	// std::cout << b.intersectP(r, Vec3f(5.f, 5.f, 5.f), neg) << std::endl;

	// render_ppm(400, 400);
	// Mat4x4f m{400.f, 0.f, 0.f, 400.f,
	// 		  0.f, -400.f, 0.f, 400.f,
	// 		  0.f, 0.f, 1.f, 0.f,
	// 		  0.f, 0.f, 0.f, 1.f};
	// std::cout << Inverse(m) << std::endl;
	// Transform trans(m);
	// std::cout << trans << std::endl;
	// std::cout << "-------------------" << std::endl;
	// std::cout << Inverse(trans) << std::endl;
	// OBJ_loader_BVH_test(800, 800);
	// test_pcg();
	// std::cout << std::is_same<Vec3<int>, Vec3i>::value << std::endl;
	return 0;
}