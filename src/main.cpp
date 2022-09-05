#include <iostream>
#include "core/geometry.h"

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

	Point3<float> p1{2.0f, 2.0f, 2.0f}, p2;
	Vec3<float> v1{3.0f, 4.0f, 5.0f}, v2;

	p2 = (Point3<float>)v1;
	v2 = (Vec3<float>)p1;

	std::cout << "p2: " << p2 << std::endl;
	std::cout << "v2: " << v2 << std::endl;

	std::cout << v1.normalized() << std::endl;
	std::cout << v1.Length() << std::endl;
	return 0;
}