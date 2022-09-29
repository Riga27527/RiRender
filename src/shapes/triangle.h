#pragma once

#include "shape.h"
#include <string>
#include <unordered_map>

RIGA_NAMESPACE_BEGIN

struct TriangleMesh{
	TriangleMesh(){}
	TriangleMesh(const Transform& Obj2Wor, 
		int nTris, int nVers,
		const int* vIndices, const Point3f* P, 
		const Normal3f* N, const Vec3f* S, 
		const Point2f* UV, const int* fIndices);

	int nTriangles = 0, nVertices = 0;
	std::vector<int> vertexIndices;
	std::unique_ptr<Point3f[]> 	p;
	std::unique_ptr<Normal3f[]> n;
	std::unique_ptr<Vec3f[]> 	s;
	std::unique_ptr<Point2f[]> 	uv;
	std::vector<int> faceIndices;
};


class WavefrontOBJ : public TriangleMesh{
public:
	struct OBJVertex {
        uint32_t p = (uint32_t) -1;
        uint32_t n = (uint32_t) -1;
        uint32_t uv = (uint32_t) -1;

        inline OBJVertex() { }

        inline OBJVertex(const std::string &string) {
            std::vector<std::string> tokens = tokenize(string, "/", true);

            if (tokens.size() < 1 || tokens.size() > 3)
                throw("Invalid vertex data ! ");

            p = toUInt(tokens[0]);

            if (tokens.size() >= 2 && !tokens[1].empty())
                uv = toUInt(tokens[1]);

            if (tokens.size() >= 3 && !tokens[2].empty())
                n = toUInt(tokens[2]);
        }

        inline bool operator==(const OBJVertex &v) const {
            return v.p == p && v.n == n && v.uv == uv;
        }
    };

    /// Hash function for OBJVertex
    struct OBJVertexHash {
        std::size_t operator()(const OBJVertex &v) const {
            size_t hash = std::hash<uint32_t>()(v.p);
            hash = hash * 37 + std::hash<uint32_t>()(v.uv);
            hash = hash * 37 + std::hash<uint32_t>()(v.n);
            return hash;
        }
    };

    typedef std::unordered_map<OBJVertex, uint32_t, OBJVertexHash> VertexMap;   

	WavefrontOBJ(const Transform& Obj2Wor, const std::string& filePath);	
};

static long long triMeshBytes = 0;
static long long total_Tris = 0;
static long long total_Meshs = 0;

class Triangle : public Shape{
public:
	Triangle(const Transform* obj2wor, const Transform* wor2obj, bool reverseOrient,
		const std::shared_ptr<TriangleMesh>& m, size_t triIndex) 
		: Shape(obj2wor, wor2obj, reverseOrient), mesh(m){
		vIndex = &mesh->vertexIndices[triIndex * 3];
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
	const int* vIndex;
	int faceIndex;
};

std::vector<std::shared_ptr<Shape>> CreateTriangleMesh(
	const Transform* obj2wor, const Transform* wor2obj, bool reverseOrient,
	int nTris, int nVers, const int* vIndices, const Point3f* P, 
	const Normal3f* N, const Vec3f* S, const Point2f* UV, const int* fIndices = nullptr);

std::vector<std::shared_ptr<Shape>> CreateOBJMesh(
	const Transform* obj2wor, const Transform* wor2obj, bool reverseOrient, std::string filePath);

RIGA_NAMESPACE_END
