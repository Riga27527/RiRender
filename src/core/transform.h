#pragma once

#include "common.h"
#include "geometry.h"
#include "interaction.h"
#include <string.h>

RIGA_NAMESPACE_BEGIN

struct Mat4x4f{
	Mat4x4f(){ }
	Mat4x4f(float mat[4][4]){ memcpy(m, mat, 16 * sizeof(float)); }
	Mat4x4f(float t00, float t01, float t02, float t03,
			float t10, float t11, float t12, float t13,
			float t20, float t21, float t22, float t23,
			float t30, float t31, float t32, float t33){
		m[0][0] = t00;	m[0][1] = t01;	m[0][2] = t02;	m[0][3] = t03;
		m[1][0] = t10;	m[1][1] = t11;	m[1][2] = t12;	m[1][3] = t13;
		m[2][0] = t20;	m[2][1] = t21;	m[2][2] = t22;	m[2][3] = t23;
		m[3][0] = t30;	m[3][1] = t31;	m[3][2] = t32;	m[3][3] = t33;
	}
	bool operator==(const Mat4x4f& mat) const{
		for(size_t i=0; i<4; ++i)
			for(size_t j=0; j<4; ++j)
				if(m[i][j] != mat.m[i][j])
					return false;
		return true;
	}
	bool operator!=(const Mat4x4f& mat) const{
		for(size_t i=0; i<4; ++i)
			for(size_t j=0; j<4; ++j)
				if(m[i][j] != mat.m[i][j])
					return true;
		return false;
	}
	float* const operator[](size_t i){
		if(i<0 || i>3)
			throw std::out_of_range("Mat4x4f out of range!");
		return m[i];
	}
	const float* operator[](size_t i) const{
		if(i<0 || i>3)
			throw std::out_of_range("Mat4x4f out of range!");
		return m[i];
	}
	bool isIdentity() const{
		return (m[0][0] == 1.f && m[0][1] == 0.f && m[0][2] == 0.f &&
                m[0][3] == 0.f && m[1][0] == 0.f && m[1][1] == 1.f &&
                m[1][2] == 0.f && m[1][3] == 0.f && m[2][0] == 0.f &&
                m[2][1] == 0.f && m[2][2] == 1.f && m[2][3] == 0.f &&
                m[3][0] == 0.f && m[3][1] == 0.f && m[3][2] == 0.f &&
                m[3][3] == 1.f);
	}
	static Mat4x4f identity(){
		Mat4x4f mat;
        mat.m[0][0] = mat.m[1][1] = mat.m[2][2] = mat.m[3][3] = 1.f;
        mat.m[0][1] = mat.m[0][2] = mat.m[0][3] = 
        mat.m[1][0] = mat.m[1][2] = mat.m[1][3] = 
        mat.m[2][0] = mat.m[2][1] = mat.m[2][3] = 
        mat.m[3][0] = mat.m[3][1] = mat.m[3][2] = 0.f;
        return mat;
	}
	friend Mat4x4f Transpose(const Mat4x4f& mat);
	friend Mat4x4f Inverse(const Mat4x4f& m);
	friend Mat4x4f Mul(const Mat4x4f& m1, const Mat4x4f& m2);
	friend std::ostream& operator<<(std::ostream& os, const Mat4x4f& mat);

	float m[4][4] = {0};
};


class Transform{
public:
	Transform() : m(Mat4x4f::identity()), mInv(Mat4x4f::identity()){}
	Transform(const Mat4x4f& mat) : m(mat), mInv(Inverse(mat)){}
	Transform(float mat[4][4]) : m(Mat4x4f(mat)), mInv(Inverse(m)){}
	Transform(const Mat4x4f& mat, const Mat4x4f& matInv) : m(mat), mInv(matInv){}

	bool operator==(const Transform& t) const{
		return (m == t.m) && (mInv == t.mInv);
	}
	bool operator!=(const Transform& t) const{
		return (m != t.m) || (mInv != t.mInv);
	}
	Transform operator*(const Transform& t) const{
		return Transform(Mul(m, t.m), Mul(t.mInv, mInv));
	}
	bool isIdentity() const{
		return m.isIdentity();
	}
	bool hasScale() const{
		float la = (*this)(Vec3f(1.0f, 0.f, 0.f)).lengthSquared();
		float lb = (*this)(Vec3f(0.f, 1.0f, 0.f)).lengthSquared();
		float lc = (*this)(Vec3f(0.f, 0.f, 1.0f)).lengthSquared();
		return (la < 0.999f || la > 1.001f) || (lb < 0.999f || lb > 1.001f) || (lc < 0.999f || lc > 1.001f);
	}
	const Mat4x4f& getMatrix() const{
		return m;
	}
	const Mat4x4f& getInverseMatrix() const{
		return mInv;
	}
	template <class T>
	Vec3<T> operator()(const Vec3<T>& v) const{
		return Vec3<T>(
			m[0][0] * v[0] + m[0][1] * v[1] + m[0][2] * v[2],
			m[1][0] * v[0] + m[1][1] * v[1] + m[1][2] * v[2],
			m[2][0] * v[0] + m[2][1] * v[1] + m[2][2] * v[2]
			);
	}
	template <class T>
	Point3<T> operator()(const Point3<T>& p) const{
		T x = m[0][0] * p[0] + m[0][1] * p[1] + m[0][2] * p[2] + m[0][3];
		T y = m[1][0] * p[0] + m[1][1] * p[1] + m[1][2] * p[2] + m[1][3];
		T z = m[2][0] * p[0] + m[2][1] * p[1] + m[2][2] * p[2] + m[2][3];
		T w = m[3][0] * p[0] + m[3][1] * p[1] + m[3][2] * p[2] + m[3][3];
		if(w == 0){
			std::cout << "Transform with point w == 0!!!\n";
			return Point3<T>(x, y, z);
		}
		return Point3<T>(x, y, z) / w;
	}
	template <class T>
	Normal3<T> operator()(const Normal3<T>& n) const{
		return Normal3<T>(
			m[0][0] * n[0] + m[0][1] * n[1] + m[0][2] * n[2],
			m[1][0] * n[0] + m[1][1] * n[1] + m[1][2] * n[2],
			m[2][0] * n[0] + m[2][1] * n[1] + m[2][2] * n[2]
			);		
	}
	template<class T>
	Bounds3<T> operator()(const Bounds3<T>& b) const{
	   	const Transform &M = *this;
	    Bounds3f ret(M(Point3f(b.pMin.x, b.pMin.y, b.pMin.z)));
	    ret = Union(ret, M(Point3f(b.pMax.x, b.pMin.y, b.pMin.z)));
	    ret = Union(ret, M(Point3f(b.pMin.x, b.pMax.y, b.pMin.z)));
	    ret = Union(ret, M(Point3f(b.pMin.x, b.pMin.y, b.pMax.z)));
	    ret = Union(ret, M(Point3f(b.pMin.x, b.pMax.y, b.pMax.z)));
	    ret = Union(ret, M(Point3f(b.pMax.x, b.pMax.y, b.pMin.z)));
	    ret = Union(ret, M(Point3f(b.pMax.x, b.pMin.y, b.pMax.z)));
	    ret = Union(ret, M(Point3f(b.pMax.x, b.pMax.y, b.pMax.z)));
	    return ret;
	}
	Ray operator()(const Ray& r) const{
		return Ray((*this)(r.o), (*this)(r.dir), r.tMin, r.tMax, r.time);
	}
	SurfaceInteraction operator()(const SurfaceInteraction& isec) const{
		SurfaceInteraction ret;
		const Transform& t = (*this);

		ret.p = t(isec.p);
		ret.n = Normalize(t(isec.n));
		ret.wo = Normalize(t(isec.wo));
		ret.time = isec.time;
		ret.uv = isec.uv;
		ret.shape = isec.shape;
		ret.shading.n = Normalize(t(isec.shading.n)).faceForward(ret.n);
		ret.primitive = isec.primitive;

		return ret;
	} 
	friend Transform Inverse(const Transform& t){
		return Transform(t.mInv, t.m);
	}
	friend Transform Transpose(const Transform& t){
		return Transform(Transpose(t.m), Transpose(t.mInv));
	}
	friend std::ostream& operator<<(std::ostream& os, const Transform& t);
private:
	Mat4x4f m, mInv;
};

Transform Translate(const Vec3f& v);
Transform Scale(const Vec3f& v);
Transform Scale(float x, float y, float z);
Transform RotateX(float theta);
Transform RotateY(float theta);
Transform RotateZ(float theta);
Transform Rotate(float theta, const Vec3f& axis);
Transform LookAt(const Point3f& pos, const Point3f& focus, const Vec3f& up);
Transform Orthographic(float znear, float zfar);
Transform Perspective(float fov, float znear, float zfar);

RIGA_NAMESPACE_END