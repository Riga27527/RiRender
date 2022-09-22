#include "triangle.h"
#include <fstream>
#include <sstream>

RIGA_NAMESPACE_BEGIN

TriangleMesh::TriangleMesh(
	const Transform& Obj2Wor, 
	int nTris, int nVers, 
	const int* vIndices, const Point3f* P, 
	const Normal3f* N, const Vec3f* S, 
	const Point2f* UV, const int* fIndices) 
	: nTriangles(nTris), nVertices(nVers), 
	  vertexIndices(vIndices, vIndices + 3 * nTris){
	
	total_Meshs++;
	total_Tris += nTris;

	p.reset(new Point3f[nVertices]);
	for(size_t i=0; i<nVertices; ++i)
		p[i] = Obj2Wor(P[i]);

	if(UV){
		uv.reset(new Point2f[nVertices]);
		memcpy(uv.get(), UV, nVertices * sizeof(Point2f));
	}
	if(N){
		n.reset(new Normal3f[nVertices]);
		for(size_t i=0; i<nVertices; ++i)
			n[i] = Obj2Wor(N[i]);
	}
	if(S){
		s.reset(new Vec3f[nVertices]);
		for(size_t i=0; i<nVertices; ++i)
			s[i] = Obj2Wor(S[i]);
	}
	if(fIndices)
		faceIndices = std::vector<int>(fIndices, fIndices + nTris);

}

WavefrontOBJ::WavefrontOBJ(const Transform& Obj2Wor, const std::string& filePath){
		std::ifstream is(filePath);
        if (is.fail())
            throw("Unable to open OBJ file !");

        std::cout << "Loading \"" << filePath << "\" .. ";
        std::cout.flush();
        TICK(OBJ_LOAD)

        std::vector<Point3f>   positions;
        std::vector<Point2f>   texcoords;
        std::vector<Normal3f>  normals;
        std::vector<int>       indices;
        std::vector<OBJVertex> vertices;
        std::vector<int>	   f_indices;
        VertexMap vertexMap;

        std::string line_str;
        while (std::getline(is, line_str)) {
            std::istringstream line(line_str);
            std::string prefix;
            line >> prefix;

            if (prefix == "v") {
                Point3f p;
                line >> p.x >> p.y >> p.z;
                positions.push_back(Obj2Wor(p));
            } else if (prefix == "vt") {
                Point2f tc;
                line >> tc.x >> tc.y;
                texcoords.push_back(tc);
            } else if (prefix == "vn") {
                Normal3f n;
                line >> n.x >> n.y >> n.z;
                normals.push_back(Obj2Wor(n.normalized()));
            } else if (prefix == "f") {
                std::string v1, v2, v3, v4;
                line >> v1 >> v2 >> v3 >> v4;
                OBJVertex verts[6];
                int nVertices = 3;

                verts[0] = OBJVertex(v1);
                verts[1] = OBJVertex(v2);
                verts[2] = OBJVertex(v3);

                if (!v4.empty()) {
                    /* This is a quad, split into two triangles */
                    verts[3] = OBJVertex(v4);
                    verts[4] = verts[0];
                    verts[5] = verts[2];
                    nVertices = 6;
                }
                /* Convert to an indexed vertex list */
                for (int i=0; i<nVertices; ++i) {
                    const OBJVertex &v = verts[i];
                    VertexMap::const_iterator it = vertexMap.find(v);
                    if (it == vertexMap.end()) {
                        vertexMap[v] = (uint32_t) vertices.size();
                        indices.push_back((uint32_t) vertices.size());
                        vertices.push_back(v);
                    } else {
                        indices.push_back(it->second);
                    }
                }
            }
        }

        nTriangles = indices.size() / 3;
        nVertices = vertices.size();
       	vertexIndices = indices;

        p.reset(new Point3f[nVertices]);
        for (uint32_t i=0; i<nVertices; ++i)
            p[i] = positions.at(vertices[i].p-1);

        if (!normals.empty()) {
        	n.reset(new Normal3f[nVertices]);
            for (uint32_t i=0; i<nVertices; ++i)
                n[i] = normals.at(vertices[i].n-1);
        }

        if (!texcoords.empty()) {
        	uv.reset(new Point2f[nVertices]);
            for (uint32_t i=0; i<nVertices; ++i)
                uv[i] = texcoords.at(vertices[i].uv-1);
        }

        std::cout << "done. (V=" << nVertices << ", F=" << nTriangles << std::endl;	
        TOCK(OBJ_LOAD)
}

Bounds3f Triangle::worldBound() const{
	const Point3f& v0 = mesh->p[vIndex[0]];
	const Point3f& v1 = mesh->p[vIndex[1]];
	const Point3f& v2 = mesh->p[vIndex[2]];
	return Union(Bounds3f(v0, v1), v2);
}

Bounds3f Triangle::objectBound() const{
	return (*world2Object)(worldBound());
}

bool Triangle::intersect(const Ray& ray, float *tHit, SurfaceInteraction* isect) const{
	// Moller Trumbore
	const Point3f& v0 = mesh->p[vIndex[0]];
	const Point3f& v1 = mesh->p[vIndex[1]];
	const Point3f& v2 = mesh->p[vIndex[2]];	

	Vec3f e1 = v1 - v0, e2 = v2 - v0;
	Vec3f s1 = Cross(ray.dir, e2);
	float det = Dot(e1, s1);

	// if determinant is near zero, ray lies in plane of triangle
	if(det > -1e-8f && det < 1e-8f)
		return false;
	float inv_det = 1.f / det;

	// compute u
	Vec3f s = ray.o - v0;
	float u = Dot(s, s1) * inv_det;
	if(u < 0.f || u > 1.f)
		return false;

	// compute v
	Vec3f s2 = Cross(s, e1);
	float v = Dot(s2, ray.dir) * inv_det;
	if(v < 0.f || u + v > 1.f)
		return false;

	// compute t
	float t = Dot(s2, e2) * inv_det;
	if(t > ray.tMax || t < 0)
		return false;

	*tHit = t;
	Point2f uv(u, v);
	Vec3f bary(1.f - u - v, u, v);

	Point3f pHit = v0 * bary.x + v1 * bary.y + v2 * bary.z;
	Normal3f geo_normal = Normal3f(Normalize(Cross((v1 - v0), (v2 - v0))));
	if(mesh->uv){
		Point2f uv0 = mesh->uv[vIndex[0]];
		Point2f uv1 = mesh->uv[vIndex[1]];
		Point2f uv2 = mesh->uv[vIndex[2]];
		uv = uv0 * bary.x + uv1 * bary.y + uv2 * bary.z;
	}

	*isect = SurfaceInteraction(pHit, geo_normal, -ray.dir, ray.time, uv, this);
	if(mesh->n){
		Normal3f n0 = mesh->n[vIndex[0]];
		Normal3f n1 = mesh->n[vIndex[1]];
		Normal3f n2 = mesh->n[vIndex[2]];
		Normal3f shading_normal = Normalize(n0 * bary.x + n1 * bary.y + n2 * bary.z);
		isect->setShadingInfo(shading_normal);
	}
	return true;
}

bool Triangle::intersectP(const Ray& ray) const{
	// Moller Trumbore
	const Point3f& v0 = mesh->p[vIndex[0]];
	const Point3f& v1 = mesh->p[vIndex[1]];
	const Point3f& v2 = mesh->p[vIndex[2]];	

	Vec3f e1 = v1 - v0, e2 = v2 - v0;
	Vec3f s1 = Cross(ray.dir, e2);
	float det = Dot(e1, s1);

	// if determinant is near zero, ray lies in plane of triangle
	if(det > -1e-8f && det < 1e-8f)
		return false;
	float inv_det = 1.f / det;

	// compute u
	Vec3f s = ray.o - v0;
	float u = Dot(s, s1) * inv_det;
	if(u < 0.f || u > 1.f)
		return false;

	// compute v
	Vec3f s2 = Cross(s, e1);
	float v = Dot(s2, ray.dir) * inv_det;
	if(v < 0.f || u + v > 1.f)
		return false;

	// compute t
	float t = Dot(s2, e2) * inv_det;
	if(t > ray.tMax || t < 0)
		return false;

	return true;
}

float Triangle::Area() const{
	const Point3f& v0 = mesh->p[vIndex[0]];
	const Point3f& v1 = mesh->p[vIndex[1]];
	const Point3f& v2 = mesh->p[vIndex[2]];
	return 0.5f * Cross(v1 - v0, v2 - v0).length();
}

std::vector<std::shared_ptr<Shape>> CreateTriangleMesh(
	const Transform* obj2wor, const Transform* wor2obj, bool reverseOrient,
	int nTris, int nVers, const int* vIndices, const Point3f* P, 
	const Normal3f* N, const Vec3f* S, const Point2f* UV, const int* fIndices){
	
	std::shared_ptr<TriangleMesh> mesh = std::make_shared<TriangleMesh>(
		*obj2wor, nTris, nVers, vIndices, P, N, S, UV, fIndices);

	std::vector<std::shared_ptr<Shape>> triangles;
	triangles.reserve(nTris);
	for(size_t i=0; i<nTris; ++i)
		triangles.push_back(std::make_shared<Triangle>(obj2wor, wor2obj, reverseOrient, mesh, i));
	return triangles;
}

std::vector<std::shared_ptr<Shape>> CreateOBJMesh(
	const Transform* obj2wor, const Transform* wor2obj, bool reverseOrient, std::string filePath){

	std::shared_ptr<TriangleMesh> mesh = std::make_shared<WavefrontOBJ>(*obj2wor, filePath);

	std::vector<std::shared_ptr<Shape>> triangles;
	triangles.reserve(mesh->nTriangles);
	for(size_t i=0; i<mesh->nTriangles; ++i)
		triangles.push_back(std::make_shared<Triangle>(obj2wor, wor2obj, reverseOrient, mesh, i));
	return triangles;
}


RIGA_NAMESPACE_END