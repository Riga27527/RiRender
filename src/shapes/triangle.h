#pragma once

#include "../core/shape.h"

RIGA_NAMESPACE_BEGIN

struct TriangleMesh{
	TriangleMesh(const Transform& Obj2Wor, 
		int nTris, int nVers,
		const int* vIndices, const Point3f* P, 
		const Normal3f* N, const Vec3f* S, 
		const Point2f* UV, const int* fIndices);

	const int nTriangles, nVertices;
	std::vector<int> vertexIndices;
	std::unique_ptr<Point3f[]> 	p;
	std::unique_ptr<Normal3f[]> n;
	std::unique_ptr<Vec3f[]> 	s;
	std::unique_ptr<Point2f[]> 	uv;
	std::vector<int> faceIndices;
};

static long long triMeshBytes = 0;
static long long total_Tris = 0;
static long long total_Meshs = 0;

class Triangle : public Shape{
public:
	Triangle(const Transform* obj2wor, const Transform* wor2obj, bool reverseOrient,
		const std::shared_ptr<TriangleMesh>& m, size_t triIndex) 
		: Shape(obj2wor, wor2obj, reverseOrient), mesh(m){
		v = &mesh->vertexIndices[triIndex * 3];
		faceIndex = mesh->faceIndices.size() ? mesh->faceIndices[triIndex] : 0;
		triMeshBytes += sizeof(*this);
	}
	Bounds3f worldBound() const;
	Bounds3f objectBound() const;
	bool intersect(const Ray& ray, float *tHit, SurfaceInteraction* isect) const;
	bool intersectP(const Ray& ray) const;
	float Area() const;

private:
	std::shared_ptr<TriangleMesh> mesh;
	const int* v;
	int faceIndex;
};

std::vector<std::shared_ptr<Shape>> CreateTriangleMesh(
	const Transform* obj2wor, const Transform* wor2obj, bool reverseOrient,
	int nTris, int nVers, const int* vIndices, const Point3f* P, 
	const Normal3f* N, const Vec3f* S, const Point2f* UV, const int* fIndices = nullptr);

RIGA_NAMESPACE_END
