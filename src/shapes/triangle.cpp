#include "triangle.h"

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

Bounds3f Triangle::worldBound() const{
	const Point3f& v0 = mesh->p[v[0]];
	const Point3f& v1 = mesh->p[v[1]];
	const Point3f& v2 = mesh->p[v[2]];
	return Union(Bounds3f(v0, v1), v2);
}

Bounds3f Triangle::objectBound() const{
	return (*world2Object)(worldBound());
}

bool Triangle::intersect(const Ray& ray, float *tHit, SurfaceInteraction* isect) const{
	// Moller Trumbore
	const Point3f& v0 = mesh->p[v[0]];
	const Point3f& v1 = mesh->p[v[1]];
	const Point3f& v2 = mesh->p[v[2]];	

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
	// Point3f pHit = ray.at(t);
	// *isect = (*object2World)(SurfaceInteraction(pHit, -ray.dir, ray.time, this));
	return true;
}

bool Triangle::intersectP(const Ray& ray) const{
	// Moller Trumbore
	const Point3f& v0 = mesh->p[v[0]];
	const Point3f& v1 = mesh->p[v[1]];
	const Point3f& v2 = mesh->p[v[2]];	

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
	const Point3f& v0 = mesh->p[v[0]];
	const Point3f& v1 = mesh->p[v[1]];
	const Point3f& v2 = mesh->p[v[2]];
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

RIGA_NAMESPACE_END