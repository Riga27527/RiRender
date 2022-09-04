#include <iostream>
#include "core/common.h"

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
	return 0;
}