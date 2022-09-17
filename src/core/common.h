#pragma once

#include <iostream>
#include <chrono>
#include <cmath>
#include <tbb/tick_count.h>
#include <numeric>
// #include "shape.h"

#define RIGA_NAMESPACE_BEGIN namespace riga {
#define RIGA_NAMESPACE_END }

#define TICK(x) auto bench_##x = std::chrono::steady_clock::now();
#define TOCK(x) std::cout << #x ": " << \
std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - bench_##x).count() << "s" << std::endl;

#define TICK_TBB(x) auto bench_##x = tbb::tick_count::now();
#define TOCK_TBB(x) std::cout << #x ": " << (tbb::tick_count::now() - bench_##x).seconds() << "s" << std::endl;

#define Infinity std::numeric_limits<float>::infinity()
// #define Epsilon  std::numeric_limits<float>::epsilon()

RIGA_NAMESPACE_BEGIN

static constexpr float PI         	= 3.14159265358979323846f;
static constexpr float INV_PI       = 0.31830988618379067154f;
static constexpr float INV_TWOPI    = 0.15915494309189533577f;
static constexpr float INV_FOURPI   = 0.07957747154594766788f;
static constexpr float SQRT_TWO     = 1.41421356237309504880f;
static constexpr float INV_SQRT_TWO = 0.70710678118654752440f;

template <class T>
class Vec2;
template <class T>
class Vec3;
template <class T>
class Point2;
template <class T>
class Point3;
template <class T>
class Normal3;
template <class T>
class Bounds2;
template <class T>
class Bounds3;

class Transform;
class Shape;
class Primitive;
class GeometricPrimitive;
struct Interaction;
class SurfaceInteraction;
class Sphere;
struct TriangleMesh;
class Triangle;

template <class T>
bool isNaN(const T x){
	return std::isnan(x);
}	

template <class T>
bool isInf(const T x){
	return std::isinf(x);
}

inline float Radians(float deg){
	return (PI / 180.f) * deg;
}

inline float Degrees(float rad){
	return (180.f / PI) * rad;
}

inline float Clamp(float x, float small, float large){
	return std::min(std::max(x, small), large);
}
RIGA_NAMESPACE_END