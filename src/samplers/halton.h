#pragma once
#include "common.h"
#include "sampler.h"
#include "lowdiscrepancy.h"

RIGA_NAMESPACE_BEGIN
class HaltonSampler : public GlobalSampler{
  public:
    // HaltonSampler Public Methods
    HaltonSampler(int nsamp, const Bounds2i &sampleBounds,
                  bool sampleAtCenter = false);
    int64_t getIndexForSample(int64_t sampleNum) const;
    float sampleDimension(int64_t index, int dimension) const;
    std::unique_ptr<Sampler> clone(int seed);

  private:
    // HaltonSampler Private Data
    static std::vector<uint16_t> radicalInversePermutations;
    Point2i baseScales, baseExponents;
    int sampleStride;
    int multInverse[2];
    mutable Point2i pixelForOffset = Point2i(std::numeric_limits<int>::max(),
                                             std::numeric_limits<int>::max());
    mutable int64_t offsetForCurrentPixel;
    // Added after book publication: force all image samples to be at the
    // center of the pixel area.
    bool sampleAtPixelCenter;

    // HaltonSampler Private Methods
    const uint16_t *permutationForDimension(int dim) const {
        if (dim >= PrimeTableSize)
            throw("HaltonSampler can only sample %d dimensions.", &PrimeTableSize);
        return &radicalInversePermutations[PrimeSums[dim]];
    }
};

HaltonSampler *CreateHaltonSampler(int pixelSamples, bool sampleAtCenter,
                                   const Bounds2i &sampleBounds);
RIGA_NAMESPACE_END