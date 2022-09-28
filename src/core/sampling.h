#pragma once
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

RIGA_NAMESPACE_END
