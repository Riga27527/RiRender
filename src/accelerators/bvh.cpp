#include "bvh.h"
#include <vector>
#include <algorithm>
#include <stack>

RIGA_NAMESPACE_BEGIN

BVH::BVH(std::vector<std::shared_ptr<Primitive>> prims, 
		int maxPrimsInNode, SplitMethod method)
		: Aggregate(std::move(prims)), 
		  maxPrimsInNode(maxPrimsInNode), 
		  splitMethod(method){
	if(primitives.empty())	return;

	std::vector<BVHPrimInfo> primInfo(primitives.size());
	for(size_t i=0; i<primitives.size(); ++i)
		primInfo[i] = {i, primitives[i]->worldBound()};

	std::vector<std::shared_ptr<Primitive>> orderedPrims;
	orderedPrims.reserve(primitives.size());
	int total_nodes = 0;
	BVHNode* node = recursiveBuild(primInfo, &total_nodes, 0, primitives.size(), orderedPrims);
	primitives.swap(orderedPrims);
	// construct a empty object to clear primInfo capacity
	// std::vector<BVHPrimInfo>().swap(primInfo);
	primInfo.resize(0);

	root = new LinearBVHNode[total_nodes];
	int offset = 0;
	flattenBVHTree(node, &offset);
}

BVH::~BVH(){}

BVHNode* BVH::recursiveBuild(std::vector<BVHPrimInfo>& primInfo, int* total_nodes,
	int start, int end, std::vector<std::shared_ptr<Primitive>>& orderedPrims){
	BVHNode* node = new BVHNode();
	(*total_nodes)++;

	Bounds3f bounds;
	for(size_t i=start; i<end; ++i)
		bounds = Union(bounds, primInfo[i].bounds);
	int nPrims = end - start;
	// std::cout << *(total_nodes) << std::endl;

	// 
	if(nPrims == 1){
		int firstPrimIndex = orderedPrims.size();
		for(size_t i=start; i<end; ++i){
			size_t index = primInfo[i].primIndex;
			orderedPrims.push_back(primitives[index]);
		}
		node->initLeaf(firstPrimIndex, nPrims, bounds);
		return node;
	}else{
		Bounds3f centroidBounds;
		for(size_t i=start; i<end; ++i)
			centroidBounds = Union(centroidBounds, primInfo[i].centroid);
		int splitDim = centroidBounds.maxExtent();

		//
		if(centroidBounds.pMin[splitDim] == centroidBounds.pMax[splitDim]){
			int firstPrimIndex = orderedPrims.size();
			for(size_t i=start; i<end; ++i){
				size_t index = primInfo[i].primIndex;
				orderedPrims.push_back(primitives[index]);
			}
			node->initLeaf(firstPrimIndex, nPrims, bounds);	
			return node;		
		}else{
			int mid = (start + end) / 2;
			switch(splitMethod) {
				case SplitMethod::Middle:	break;
				case SplitMethod::EqualCounts : break;
				case SplitMethod::SAH:
				default:{
					if(nPrims <= 4){
						mid = (start + end) / 2;
						std::nth_element(&primInfo[start], &primInfo[mid], &primInfo[end-1] + 1, 
							[splitDim](const BVHPrimInfo& p1, const BVHPrimInfo& p2){
								return p1.centroid[splitDim] < p2.centroid[splitDim];
							});
					}else{
						constexpr int nBuckets = 12;
						BucketInfo buckets[nBuckets];

						// 
						for(size_t i=start; i<end; ++i){
							int bucket_index = static_cast<int>(nBuckets * 
								centroidBounds.offset(primInfo[i].centroid)[splitDim]);
							if(bucket_index == nBuckets)
								bucket_index = nBuckets - 1;
							buckets[bucket_index].primCount++;
							buckets[bucket_index].bucketBounds 
								= Union(buckets[bucket_index].bucketBounds, primInfo[i].bounds);
						}

						float minCost = Infinity;
						int minBucketIndex = -1;
						float inv_wholeArea = 1.f / bounds.surfaceArea();
						for(size_t i=0; i<nBuckets-1; ++i){
							Bounds3f left_bounds, right_bounds;
							int left_count = 0, right_count = 0;
							for(size_t j=0; j<=i; ++j){
								left_count += buckets[j].primCount;
								left_bounds = Union(left_bounds, buckets[j].bucketBounds);
							}
							for(size_t j=i+1; j<nBuckets; ++j){
								right_count += buckets[j].primCount;
								right_bounds = Union(right_bounds, buckets[j].bucketBounds);
							}
							float cost = (left_count * left_bounds.surfaceArea() 
											+ right_count * right_bounds.surfaceArea()) * inv_wholeArea;
							if(cost < minCost){
								minCost = cost;
								minBucketIndex = i;
							}
						}

						float leafCost = nPrims;
						if(nPrims > maxPrimsInNode || minCost < leafCost){
							BVHPrimInfo* pmid = std::partition(&primInfo[start], &primInfo[end-1] + 1, 
									[=](const BVHPrimInfo& info){
										int bucket_index = static_cast<int>(nBuckets * 
											centroidBounds.offset(info.centroid)[splitDim]);
										return bucket_index <= minBucketIndex;
								});
							mid = pmid - (&primInfo[0]);
						}else{
							int firstPrimIndex = orderedPrims.size();
							for(size_t i=start; i<end; ++i){
								size_t index = primInfo[i].primIndex;
								orderedPrims.push_back(primitives[index]);
							}
							node->initLeaf(firstPrimIndex, nPrims, bounds);	
							return node;						
						}
					}
					break;
				}
			}
			node->initInterior(splitDim, 
				recursiveBuild(primInfo, total_nodes, start, mid, orderedPrims),
				recursiveBuild(primInfo, total_nodes, mid, end, orderedPrims));
		}
	}
	return node;
}

Bounds3f BVH::worldBound() const{
	return root ? root->bounds : Bounds3f();
}

int BVH::flattenBVHTree(BVHNode* node, int* offset){
	LinearBVHNode* linearNode = &root[*offset];
	int cur_offset = (*offset)++;
	linearNode->bounds = node->bounds;
	linearNode->nPrimitives = node->nPrimitive;
	if(node->nPrimitive > 0){
		linearNode->primIndex = node->firstPrimIndex;
	}else{
		linearNode->splitDim = node->splitAxis;
		flattenBVHTree(node->children[0], offset);
		linearNode->rightChildOffset = flattenBVHTree(node->children[1], offset);
	}
	return cur_offset;
}


bool BVH::intersect(const Ray& ray, SurfaceInteraction* isect) const{
	if(root == nullptr)
		return false;

	const Vec3f invDir(1.f / ray.dir.x, 1.f / ray.dir.y, 1.f / ray.dir.z);
	const bool isDirNeg[3] = {ray.dir.x < 0.f, ray.dir.y < 0.f, ray.dir.z < 0.f};

	bool hit = false;
	int curOffset = 0;
	std::stack<int> need2Test;
	need2Test.push(curOffset);
	while(!need2Test.empty()){
		curOffset = need2Test.top();
		need2Test.pop();
		const LinearBVHNode* curNode = &root[curOffset];

		if(!curNode->bounds.intersectP(ray, invDir, isDirNeg)){
			continue;
		}else{
			if(curNode->nPrimitives > 0){
				for(int i = 0; i < curNode->nPrimitives; ++i)
					if(primitives[curNode->primIndex + i]->intersect(ray, isect))
						hit = true;
			}else{
				if(isDirNeg[curNode->splitDim]){
					need2Test.push(curNode->rightChildOffset);
					need2Test.push(curOffset + 1);
				}else{
					need2Test.push(curOffset + 1);
					need2Test.push(curNode->rightChildOffset);
				}
			}
		}
	}
	return hit;
}

bool BVH::intersectP(const Ray& ray) const{
	if(root == nullptr)
		return false;

	const Vec3f invDir(1.f / ray.dir.x, 1.f / ray.dir.y, 1.f / ray.dir.z);
	const bool isDirNeg[3] = {ray.dir.x < 0.f, ray.dir.y < 0.f, ray.dir.z < 0.f};

	int curOffset = 0;
	std::stack<int> need2Test;
	need2Test.push(curOffset);
	while(!need2Test.empty()){
		curOffset = need2Test.top();
		need2Test.pop();
		const LinearBVHNode* curNode = &root[curOffset];

		if(!curNode->bounds.intersectP(ray, invDir, isDirNeg)){
			continue;
		}else{
			if(curNode->nPrimitives > 0){
				for(int i = 0; i < curNode->nPrimitives; ++i)
					if(primitives[i + curNode->primIndex]->intersectP(ray))
						return true;
			}else{
				need2Test.push(curOffset+1);
				need2Test.push(curNode->rightChildOffset);
			}
		}
	}
	return false;
}

RIGA_NAMESPACE_END