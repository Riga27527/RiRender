#pragma once

#include "common.h"
#include "geometry.h"

RIGA_NAMESPACE_BEGIN

class Sampler{
  public:
    // Sampler Interface
    virtual ~Sampler();
    Sampler(int64_t samplesPerPixel);
    virtual void startPixel(const Point2i &p);
    virtual float get1D() = 0;
    virtual Point2f get2D() = 0;
    CameraSample getCameraSample(const Point2i &pRaster);
    void request1DArray(int n);
    void request2DArray(int n);
    virtual int roundCount(int n) const { return n; }
    const float *get1DArray(int n);
    const Point2f *get2DArray(int n);
    virtual bool startNextSample();
    virtual std::unique_ptr<Sampler> clone(int seed) = 0;
    virtual bool setSampleNumber(int64_t sampleNum);
    
    int64_t currentSampleNumber() const { return currentPixelSampleIndex; }

    friend std::ostream& operator<<(std::ostream& os, const Sampler& sampler){
    	return os << "(" << sampler.currentPixel.x << " ," << sampler.currentPixel.y << 
    		"), sample " << sampler.currentPixelSampleIndex << "\n";
    }

    // Sampler Public Data
    const int64_t samplesPerPixel;

  protected:
    // Sampler Protected Data
    Point2i currentPixel;
    int64_t currentPixelSampleIndex;
    std::vector<int> samples1DArraySizes, samples2DArraySizes;
    std::vector<std::vector<float>> sampleArray1D;
    std::vector<std::vector<Point2f>> sampleArray2D;

  private:
    // Sampler Private Data
    size_t array1DOffset, array2DOffset;
};

class GlobalSampler : public Sampler {
  public:
    // GlobalSampler Public Methods
    GlobalSampler(int64_t samplesPerPixel) : Sampler(samplesPerPixel) {}
    bool startNextSample();
    void startPixel(const Point2i &);
    bool setSampleNumber(int64_t sampleNum);
    float get1D();
    Point2f get2D();
    virtual int64_t getIndexForSample(int64_t sampleNum) const = 0;
    virtual float sampleDimension(int64_t index, int dimension) const = 0;

  private:
    // GlobalSampler Private Data
    int dimension;
    int64_t intervalSampleIndex;
    static const int arrayStartDim = 2;
    int arrayEndDim;
};

RIGA_NAMESPACE_END