#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include "core/geometry.h"
#include "core/transform.h"
#include "shapes/sphere.h"

using namespace riga;

void render_ppm(int width, int height){
	Transform sphereT_obj2wor, sphereT_wor2obj;
	sphereT_obj2wor = sphereT_obj2wor * Translate(Vec3f(1.0f, 0.f, 0.f));
	sphereT_wor2obj = Inverse(sphereT_obj2wor);
	std::shared_ptr<Shape> sphere = std::make_shared<Sphere>(&sphereT_obj2wor, &sphereT_wor2obj, false, 1.f);

	std::vector<Vec3f> framebuffer(width * height);
	Point3f lower_let_corner(-2.0f, -2.0f, 2.0f);
	Point3f ray_o(0.f, 0.f, 4.f);
	Vec3f horizontal(4.f, 0.f, 0.f);
	Vec3f vertical(0.f, 4.f, 0.f);
	int m = 0;
	for(int i=height-1; i>=0; --i){
		for(int j=0; j<width; ++j){	
			float u = (j + 0.5) / (width - 1);
			float v = (i + 0.5) / (height - 1);
			Point3f pos = lower_let_corner + u * horizontal + v * vertical;
			Ray r(ray_o, Normalize(pos - ray_o));
			if(sphere->intersectP(r))
				framebuffer[m++] = Vec3f(1.f, 0.f, 0.f);
			else
				framebuffer[m++] = Vec3f(1.f, 1.f, 1.f);
		}
	}


    FILE* fp = fopen("binary.ppm", "wb");
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

	render_ppm(200, 200);
	return 0;
}