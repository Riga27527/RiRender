#pragma once
#include "geometry.h"
#include <pcg32.h>

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

RIGA_NAMESPACE_END
