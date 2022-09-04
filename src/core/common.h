#pragma once

#include <iostream>
#include <chrono>
#include <tbb/tick_count.h>

#define RIGA_NAMESPACE_BEGIN namespace riga {
#define RIGA_NAMESPACE_END }

#define TICK(x) auto bench_##x = std::chrono::steady_clock::now();
#define TOCK(x) std::cout << #x ": " << \
std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - bench_##x).count() << "s" << std::endl;

#define TICK_TBB(x) auto bench_##x = tbb::tick_count::now();
#define TOCK_TBB(x) std::cout << #x ": " << (tbb::tick_count::now() - bench_##x).seconds() << "s" << std::endl;


RIGA_NAMESPACE_BEGIN
template <class T>
bool isNaN(const T x){
	return std::isnan(x);
}	

template <class T>
bool isInf(const T x){
	return std::isinf(x);
}

RIGA_NAMESPACE_END