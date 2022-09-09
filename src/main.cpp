#include <iostream>
#include "core/geometry.h"
#include "core/transform.h"

using namespace riga;

int main(int argc, char const *argv[])
{
	std::cout << "Hello Rider!" << std::endl;
	const size_t n = 100000;
	
	TICK_TBB(for)
	float m = 0;
	for(size_t i=0; i < n; ++i)
		m += i * 24 + 10;
	TOCK_TBB(for)
	std::cout << m << std::endl;

	Point3<float> p1{2.0f, 4.0f, 9.0f}, p2;
	Vec3<float> v1{3.0f, 4.0f, 5.0f}, v2;

	p2 = (Point3<float>)v1;
	v2 = (Vec3<float>)p1;

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

	auto t = LookAt(Point3f(5.f, 0.f, 0.f), Point3f(5.f, 0.f, -10.f), Vec3f(0.f, 1.0f, 0.f));
	std::cout << t << std::endl;
	return 0;
}