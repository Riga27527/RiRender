#pragma once

#include "../core/primitive.h"

RIGA_NAMESPACE_BEGIN

struct BVHNode{
	void initLeaf(int first, int n, const Bounds3f &b){
		firstPrimIndex = first;
		nPrimitive = n;
		bounds = b;
		children[0] = children[1] = nullptr;
 	}
 	void initInterior(int axis, BVHNode* left, BVHNode* right){
 		children[0] = left;
 		children[1] = right;
 		splitAxis = axis;
 		nPrimitive = 0;
 		bounds = Union(left->bounds, right->bounds);
 	}

	Bounds3f bounds;
	BVHNode* children[2];
	int splitAxis, firstPrimIndex, nPrimitive;
};

struct BVHPrimInfo{
	BVHPrimInfo(){}
	BVHPrimInfo(size_t index, const Bounds3f& b)
	: primIndex(index), 
	  bounds(b), 
	  centroid(0.5f * (b.pMin + b.pMax)){}

	size_t primIndex;
	Bounds3f bounds;
	Point3f centroid;
};

struct BucketInfo{
	int primCount = 0;
	Bounds3f bucketBounds;
};

struct LinearBVHNode{
	Bounds3f bounds;
	union {
		int rightChildOffset;	// interior
		int primIndex;			// leaf
	};

	uint16_t nPrimitives;
	uint8_t splitDim;
	uint8_t pad[1];
};

class BVH : public Aggregate{
public:
	enum class SplitMethod {SAH, Middle, EqualCounts};
	BVH(std::vector<std::shared_ptr<Primitive>> prims, 
		int maxPrimsInNode = 1, SplitMethod method = SplitMethod::SAH);
	~BVH();
	Bounds3f worldBound() const;
	bool intersect(const Ray& ray, SurfaceInteraction* isect) const;
	bool intersectP(const Ray& ray) const;

private:
	BVHNode* recursiveBuild(std::vector<BVHPrimInfo>& primInfo, int* total_nodes,
		int start, int end, std::vector<std::shared_ptr<Primitive>>& orderedPrims);
	
	int flattenBVHTree(BVHNode* node, int* offset);

	const int maxPrimsInNode;
	const SplitMethod splitMethod;
	LinearBVHNode* root = nullptr;
};


RIGA_NAMESPACE_END