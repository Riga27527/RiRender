#pragma once
#include <core/geometry.h>
#include <pcg32/pcg32.h>

RIGA_NAMESPACE_BEGIN

// Sampling Inline Functions
template <typename T>
void Shuffle(T *samp, int count, int nDimensions, pcg32 &rng) {
    for (int i = 0; i < count; ++i) {
        int other = i + rng.nextUInt(count - i);
        for (int j = 0; j < nDimensions; ++j)
            std::swap(samp[nDimensions * i + j], samp[nDimensions * other + j]);
    }
}

inline
Vec3f square2CosineHemishpere(const Point2f& sample){
    float theta = std::acos(1.f - 2.f * sample.x) / 2.f, phi = 2.f * PI * sample.y;
    return Vec3f(std::sin(theta) * std::cos(phi), std::sin(theta) * std::sin(phi), std::cos(theta)); 
}

inline
Point2f UniformSampleTriangle(const Point2f& sample){
    float tmp = std::sqrt(sample.x);
    return Point2f(1.f - tmp, sample.y * tmp);
}


RIGA_NAMESPACE_END
