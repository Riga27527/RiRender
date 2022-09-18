
#pragma once

#include "common.h"

RIGA_NAMESPACE_BEGIN

typedef Vec2<int> Vec2i;
typedef Vec2<float> Vec2f;

typedef Vec3<int> Vec3i;
typedef Vec3<float> Vec3f;

typedef Point2<int> Point2i;
typedef Point2<float> Point2f;

typedef Point3<int> Point3i;
typedef Point3<float> Point3f;

typedef Normal3<float> Normal3f;

typedef Bounds2<int> Bounds2i;
typedef Bounds2<float> Bounds2f;

typedef Bounds3<int> Bounds3i;
typedef Bounds3<float> Bounds3f;

template <class T>
class Vec2{
public:
	Vec2(){}
	Vec2(T t) : x(t), y(t){}
	Vec2(T xx, T yy) : x(xx), y(yy){}
	Vec2(const Vec2<T>& vec) : x(vec.x), y(vec.y){}
	Vec2<T>& operator=(const Vec2<T> &vec){
		x = vec.x;
		y = vec.y;
		return *this;
	}

	// Point2 -> Vec2
	explicit Vec2(const Point2<T>& p) : x(p.x), y(p.y){}

	Vec2<T>& operator+=(const Vec2<T> &vec){
		x += vec.x;
		y += vec.y;
		return *this;
	}

	Vec2<T> operator+(const Vec2<T> &vec) const{
		return Vec2<T>(x+vec.x, y+vec.y);
	}

	Vec2<T>& operator-=(const Vec2<T> &ve){
		x -= ve.x;
		y -= ve.y;
		return *this;
	}

	Vec2<T> operator-(const Vec2<T> &vec) const{
		return Vec2<T>(x-vec.x, y-vec.y);
	}

	Vec2<T> operator-() const{
		return Vec2<T>(-x, -y);
	}

	template <class U>
	Vec2<T>& operator*=(U f){
		x *= f;
		y *= f;
		return *this;
	}

	template <class U>
	Vec2<T> operator*(U f) const{
		return Vec2(x*f, y*f);
	}

	template <class U>
	Vec2<T>& operator/=(U f){
		float inv = 1.0f / f;
		x *= inv;
		y *= inv;
		return *this;
	}

	template <class U>
	Vec2<T> operator/(U f) const{
		float inv = 1.0f / f;
		return Vec2(x*inv, y*inv);
	}

	bool operator==(const Vec2<T> &vec) const{
		return (x == vec.x && y == vec.y);
	}
	bool operator!=(const Vec2<T> &vec) const{
		return (x != vec.x || y != vec.y);
	}

	T operator[](size_t i) const{
		if(i!=0 && i!=1){
			throw std::out_of_range("Vec2 index out of range\n");
		}
		return i==0 ? x : y;
	}

	T& operator[](size_t i){
		if(i!=0 && i!=1){
			throw std::out_of_range("Vec2 index out of range\n");
		}
		return i==0 ? x : y;
	}
	
	friend std::ostream& operator<<(std::ostream& os, const Vec2<T>& v){
		os << "[ " << v.x << ", " << v.y << " ]";
		return os;
	}

	float lengthSquared() const {return x*x + y*y; }
	float length() const {return std::sqrt(lengthSquared()); }	
	bool hasNaN() const {return (isNaN(x) || isNaN(y)); }
	float dot(const Vec2<T>& v) const{
		return x * v.x + y * v.y;
	}
	Vec2<T>& normalized(){
		float inv = 1.0f / length();
		x *= inv;
		y *= inv;
		return *this;
	}
	// public data
	T x = 0 , y = 0;
};


template <class T>
class Vec3{
public:
	Vec3(){}
	Vec3(T t) : x(t), y(t), z(t){}
	Vec3(T xx, T yy, T zz) : x(xx), y(yy), z(zz){}
	Vec3(const Vec3<T>& vec) : x(vec.x), y(vec.y), z(vec.z){}
	Vec3<T>& operator=(const Vec3<T> &vec){
		x = vec.x;
		y = vec.y;
		z = vec.z;
		return *this;
	}

	// Point3 -> Vec3
	explicit Vec3<T>(const Point3<T>& p) : x(p.x), y(p.y), z(p.z){}

	Vec3<T>& operator+=(const Vec3<T> &vec){
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return *this;
	}

	Vec3<T> operator+(const Vec3<T> &vec) const{
		return Vec3<T>(x+vec.x, y+vec.y, z+vec.z);
	}

	Vec3<T>& operator-=(const Vec3<T> &vec){
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		return *this;
	}

	Vec3<T> operator-(const Vec3<T> &vec) const{
		return Vec3<T>(x-vec.x, y-vec.y, z-vec.z);
	}

	Vec3<T> operator-() const{
		return Vec3<T>(-x, -y, -z);
	}

	template <class U>
	Vec3<T>& operator*=(U f){
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}

	template <class U>
	Vec3<T> operator*(U f) const{
		return Vec3(x*f, y*f, z*f);
	}

	template <class U>
	Vec3<T>& operator/=(U f){
		float inv = (float) 1 / f;
		x *= inv;
		y *= inv;
		z *= inv;
		return *this;
	}

	template <class U>
	Vec3<T> operator/(U f) const{
		float inv = (float) 1 / f;
		return Vec3(x*inv, y*inv, z*inv);
	}

	bool operator==(const Vec3<T> &vec) const{
		return (x == vec.x && y == vec.y && z == vec.z);
	}
	bool operator!=(const Vec3<T> &vec) const{
		return (x != vec.x || y != vec.y || z!=vec.z);
	}

	T operator[](size_t i) const{
		if(i<0 || i>2){
			throw std::out_of_range("Vec3 index out of range\n");
		}
		return i==0 ? x : (i==1 ? y : z);
	}

	T& operator[](size_t i){
		if(i<0 || i>2){
			throw std::out_of_range("Vec3 index out of range\n");
		}
		return i==0 ? x : (i==1 ? y : z);
	}
	friend std::ostream& operator<<(std::ostream& os, const Vec3<T>& v){
		os << "[ " << v.x << ", " << v.y << ", " << v.z << " ]";
		return os;
	}

	float lengthSquared() const {return x*x + y*y + z*z; }
	float length() const {return std::sqrt(lengthSquared()); }	
	bool hasNaN() const {return (isNaN(x) || isNaN(y) || isNaN(z)); }
	T minComponent() const {return (x < y ? (x < z ? x : z) : (y < z ? y : z)); }
	T maxComponent() const {return (x > y ? (x > z ? x : z) : (y > z ? y : z)); }
	int maxDimension() const {return (x > y ? (x > z ? 0 : 2) : (y > z ? 1 : 2)); }
	int minDimension() const {return (x < y ? (x < z ? 0 : 2) : (y < z ? 1 : 2)); }
	float dot(const Normal3<T>& n) const{
		return (x * n.x + y * n.y + z * n.z);
	}
	float dot(const Vec3<T>& v) const{
		return (x * v.x + y * v.y + z * v.z);
	}
	Vec3<T>& faceForward(const Normal3<T>& n){
		if(this->dot(n) < 0.f){
			x = -x;
			y = -y;
			z = -z;
		}
		return *this;
	}
	Vec3<T>& faceForward(const Vec3<T>& v){
		if(this->dot(v) < 0.f){
			x = -x;
			y = -y;
			z = -z;
		}
		return *this;
	}
	Vec3<T>& normalized(){
		float inv = 1.0f / length();
		x *= inv;
		y *= inv;
		z *= inv;
		return *this;
	}

	T x = 0, y = 0, z = 0;
};

template <class T>
class Point2{
public:
	Point2(){}
	Point2(T t) : x(t), y(t){}
	Point2(T xx, T yy) : x(xx), y(yy){}
	Point2(const Point2<T>& p) : x(p.x), y(p.y){}
	Point2<T>& operator=(const Point2<T> &p){
		x = p.x;
		y = p.y;
		return *this;
	}

	// Point2 -> Point2
	template<class U>
	explicit Point2(const Point2<U>& p){
		x = (T)p.x;
		y = (T)p.y;
	}

	// Vec2 -> Point2
	template<class U>
	explicit Point2(const Vec2<U>& vec){
		x = (T)vec.x;
		y = (T)vec.y;
	}

	// Point2 -> Vec2
	template<class U>
	explicit operator Vec2<U>() const{
		return Vec2<U>(x, y);
	}

	// point + point = point
	Point2<T>& operator+=(const Point2<T> &p){
		x += p.x;
		y += p.y;
		return *this;
	}

	Point2<T> operator+(const Point2<T> &p) const{
		return Point2<T>(x+p.x, y+p.y);
	}

	// point + vec = point
	Point2<T>& operator+=(const Vec2<T>& vec){
		x += vec.x;
		y += vec.y;
		return *this;
	}

	Point2<T> operator+(const Vec2<T>& vec){
		return Point2(x+vec.x, y+vec.y);
	}

	// point - point = vec
	Vec2<T> operator-(const Point2<T> &p) const{
		return Vec2<T>(x-p.x, y-p.y);
	}

	// point - vec = point
	Point2<T>& operator-=(const Vec2<T>& vec){
		x -= vec.x;
		y -= vec.y;
		return *this;
	}

	Point2<T> operator-(const Vec2<T>& vec){
		return Point2(x-vec.x, y-vec.y);
	}


	Point2<T> operator-() const{
		return Point2<T>(-x, -y);
	}

	template <class U>
	Point2<T>& operator*=(U f){
		x *= f;
		y *= f;
		return *this;
	}

	template <class U>
	Point2<T> operator*(U f) const{
		return Point2(x*f, y*f);
	}

	template <class U>
	Point2<T>& operator/=(U f){
		float inv = 1.0f / f;
		x *= inv;
		y *= inv;
		return *this;
	}

	template <class U>
	Point2<T> operator/(U f) const{
		float inv = 1.0f / f;
		return Point2(x*inv, y*inv);
	}

	bool operator==(const Point2<T> &p) const{
		return (x == p.x && y == p.y);
	}
	bool operator!=(const Point2<T> &p) const{
		return (x != p.x || y != p.y);
	}

	T operator[](size_t i) const{
		if(i!=0 && i!=1){
			throw std::out_of_range("Point2 index out of range\n");
		}
		return i==0 ? x : y;
	}

	T& operator[](size_t i){
		if(i!=0 && i!=1){
			throw std::out_of_range("Point2 index out of range\n");
		}
		return i==0 ? x : y;
	}

	friend std::ostream& operator<<(std::ostream& os, const Point2<T>& v){
		os << "[ " << v.x << ", " << v.y << " ]";
		return os;
	}
	
	bool hasNaN() const {return (isNaN(x) || isNaN(y)); }

	// public data
	T x = 0 , y = 0;
};

template <class T>
class Point3{
public:
	Point3(){}
	Point3(T t) : x(t), y(t), z(t){}
	Point3(T xx, T yy, T zz) : x(xx), y(yy), z(zz){}
	Point3(const Point3<T>& p) : x(p.x), y(p.y), z(p.z){}
	Point3<T>& operator=(const Point3<T> &p){
		x = p.x;
		y = p.y;
		z = p.z;
		return *this;
	}

	// Point3 -> Point3
	template<class U>
	explicit Point3(const Point3<U>& p){
		x = (T)p.x;
		y = (T)p.y;
		z = (T)p.z;
	}

	// Vec3 -> Point3
	template<class U>
	explicit Point3(const Vec3<U>& vec){
		x = (T)vec.x;
		y = (T)vec.y;
		z = (T)vec.z;
	}

	// Point3 -> Vec3
	template<class U>
	explicit operator Vec3<U>() const{
		return Vec3<U>(x, y, z);
	}

	// point + point = point
	Point3<T>& operator+=(const Point3<T> &p){
		x += p.x;
		y += p.y;
		z += p.z;
		return *this;
	}

	Point3<T> operator+(const Point3<T> &p) const{
		return Point3<T>(x+p.x, y+p.y, z+p.z);
	}

	// point + vec = point
	Point3<T>& operator+=(const Vec3<T>& vec){
		x += vec.x;
		y += vec.y;
		return *this;
	}

	Point3<T> operator+(const Vec3<T>& vec) const{
		return Point3(x+vec.x, y+vec.y, z+vec.z);
	}

	// point - point = vec
	Vec3<T> operator-(const Point3<T> &p) const{
		return Vec3<T>(x-p.x, y-p.y, z-p.z);
	}

	// point - vec = point
	Point3<T>& operator-=(const Point3<T>& vec){
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		return *this;
	}

	Point3<T> operator-(const Vec3<T>& vec) const{
		return Point3(x-vec.x, y-vec.y, z-vec.z);
	}


	Point3<T> operator-() const{
		return Point3<T>(-x, -y, -z);
	}

	template <class U>
	Point3<T>& operator*=(U f){
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}

	template <class U>
	Point3<T> operator*(U f) const{
		return Point3(x*f, y*f, z*f);
	}

	template <class U>
	Point3<T>& operator/=(U f){
		float inv = 1.0f / f;
		x *= inv;
		y *= inv;
		z *= inv;
		return *this;
	}

	template <class U>
	Point3<T> operator/(U f) const{
		float inv = 1.0f / f;
		return Point3(x*inv, y*inv, z*inv);
	}

	bool operator==(const Point3<T> &p) const{
		return (x == p.x && y == p.y && z == p.z);
	}
	bool operator!=(const Point3<T> &p) const{
		return (x != p.x || y != p.y || z != p.z);
	}

	T operator[](size_t i) const{
		if(i<0 || i>2){
			throw std::out_of_range("Point3 index out of range\n");
		}
		return i==0 ? x : (i==1 ? y : z);
	}

	T& operator[](size_t i){
		if(i<0 || i>2){
			throw std::out_of_range("Point3 index out of range\n");
		}
		return i==0 ? x : (i==1 ? y : z);
	}

	friend std::ostream& operator<<(std::ostream& os, const Point3<T>& v){
		os << "[ " << v.x << ", " << v.y << ", " << v.z << " ]";
		return os;
	}

	bool hasNaN() const {return (isNaN(x) || isNaN(y) || isNaN(z)); }

	// public data
	T x = 0 , y = 0, z = 0;
};


// Normal3
template <class T>
class Normal3{
public:
	Normal3(){}
	Normal3(T xx, T yy, T zz) : x(xx), y(yy), z(zz){}
	Normal3(const Normal3<T>& vec) : x(vec.x), y(vec.y), z(vec.z){}
	Normal3<T>& operator=(const Normal3<T> &vec){
		x = vec.x;
		y = vec.y;
		z = vec.z;
		return *this;
	}

	explicit Normal3<T>(const Vec3<T>& p) : x(p.x), y(p.y), z(p.z){}

	Normal3<T>& operator+=(const Normal3<T> &vec){
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return *this;
	}

	Normal3<T> operator+(const Normal3<T> &vec) const{
		return Vec3<T>(x+vec.x, y+vec.y, z+vec.z);
	}

	Normal3<T>& operator-=(const Normal3<T> &vec){
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		return *this;
	}

	Normal3<T> operator-(const Normal3<T> &vec) const{
		return Normal3<T>(x-vec.x, y-vec.y, z-vec.z);
	}

	Normal3<T> operator-() const{
		return Normal3<T>(-x, -y, -z);
	}

	template <class U>
	Normal3<T>& operator*=(U f){
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}

	template <class U>
	Normal3<T> operator*(U f) const{
		return Normal3(x*f, y*f, z*f);
	}

	template <class U>
	Normal3<T>& operator/=(U f){
		float inv = (float) 1 / f;
		x *= inv;
		y *= inv;
		z *= inv;
		return *this;
	}

	template <class U>
	Normal3<T> operator/(U f) const{
		float inv = (float) 1 / f;
		return Normal3(x*inv, y*inv, z*inv);
	}

	bool operator==(const Normal3<T> &vec) const{
		return (x == vec.x && y == vec.y && z == vec.z);
	}
	bool operator!=(const Normal3<T> &vec) const{
		return (x != vec.x || y != vec.y || z!=vec.z);
	}

	T operator[](size_t i) const{
		if(i<0 || i>2){
			throw std::out_of_range("Vec3 index out of range\n");
		}
		return i==0 ? x : (i==1 ? y : z);
	}

	T& operator[](size_t i){
		if(i<0 || i>2){
			throw std::out_of_range("Vec3 index out of range\n");
		}
		return i==0 ? x : (i==1 ? y : z);
	}
	
	friend std::ostream& operator<<(std::ostream& os, const Normal3<T>& v){
		os << "[ " << v.x << ", " << v.y << ", " << v.z << " ]";
		return os;
	}

	float lengthSquared() const {return x*x + y*y + z*z; }
	float length() const {return std::sqrt(lengthSquared()); }	
	bool hasNaN() const {return (isNaN(x) || isNaN(y) || isNaN(z)); }
	float dot(const Normal3<T>& n) const{
		return (x * n.x + y * n.y + z * n.z);
	}
	float dot(const Vec3<T>& v) const{
		return (x * v.x + y * v.y + z * v.z);
	}
	Normal3<T>& faceForward(const Normal3<T>& n){
		if(this->dot(n) < 0.f){
			x = -x;
			y = -y;
			z = -z;
		}
		return *this;
	}
	Normal3<T>& faceForward(const Vec3<T>& v){
		if(this->dot(v) < 0.f){
			x = -x;
			y = -y;
			z = -z;
		}
		return *this;
	}
	Normal3<T>& normalized(){
		float inv = 1.0f / length();
		x *= inv;
		y *= inv;
		z *= inv;
		return *this;
	}

	T x = 0, y = 0, z = 0;
};

class Ray{
public:
	Ray() : tMax(Infinity), time(0.f){}
	Ray(const Point3f& p, const Vec3f& d, float tMax = Infinity, float time = 0.f)
	: o(p), dir(d), tMax(tMax), time(time){}
	Point3f at(float t) const{
		return o + dir * t;
	}
	bool hasNaN() const{
		return (o.hasNaN() || dir.hasNaN() || isNaN(tMax)); 
	}
	friend std::ostream& operator<<(std::ostream& os, const Ray& r){
		os << "[o=" << r.o << ", dir=" << r.dir << ", tMax=" 
		   << r.tMax << ", time=" << r.time << "]";
		return os;
	}

	Point3f o;
	Vec3f dir;
	float tMax;
	float time;
};

template <class T>
class Bounds2{
public:
	Bounds2(){
		T minNum = std::numeric_limits<T>::lowest();
		T maxNum = std::numeric_limits<T>::max();
		pMin = Point2<T>(maxNum, maxNum);
		pMax = Point2<T>(minNum, minNum);
	}
	explicit Bounds2(const Point2<T>& p) : pMin(p), pMax(p){}
	Bounds2(const Point2<T>& p1, const Point2<T>& p2) 
		: pMin(std::min(p1.x, p2.x), std::min(p1.y, p2.y)),
		  pMax(std::max(p1.x, p2.x), std::max(p1.y, p2.y)){}
	template <class U>
	explicit operator Bounds2<U>() const{
		return Bounds2<U>((Point2<U>)pMin, (Point2<U>)pMax);
	}
	Vec2<T> diagnoal() const{
		return pMax - pMin;
	}
	T area() const{
		Vec2<T> diag = diagnoal();
		return (diag.x * diag.y);
	}	
	size_t maxExtent() const{
		Vec2<T> diag = diagnoal();
		return (diag.x > diag.y ? 0 : 1);
	}
	Vec2<T> offset(const Point2f& p) const{
		Vec2<T> v = p - pMin;
		v.x /= (pMax.x - pMin.x);
		v.y /= (pMax.y - pMin.y);
		return v;
	}
	const Point2<T>& operator[](size_t i) const{
		if(i!=0 && i!=1)
			throw std::out_of_range("Bounds2 index out of range\n");
		return (i==0 ? pMin : pMax);
	}
	Point2<T>& operator[](size_t i){
		if(i!=0 && i!=1)
			throw std::out_of_range("Bounds2 index out of range\n");
		return (i==0 ? pMin : pMax);
	}
	bool operator==(const Bounds2<T>& b) const{
		return (b.pMin == pMin) && (b.pMax == pMax);
	}
	bool operator!=(const Bounds2<T>& b) const{
		return (b.pMin != pMin) || (b.pMax != pMax);
	}

	friend std::ostream& operator<<(std::ostream& os, Bounds2<T> b){
		os << "[ " << b.pMin << ", " << b.pMax << " ]";
		return os;
	}
	
	Point2<T> pMin, pMax;
};


template <class T>
class Bounds3{
public:
	Bounds3(){
		T minNum = std::numeric_limits<T>::lowest();
		T maxNum = std::numeric_limits<T>::max();
		pMin = Point3<T>(maxNum, maxNum, maxNum);
		pMax = Point3<T>(minNum, minNum, minNum);
	}
	explicit Bounds3(const Point3<T>& p) : pMin(p), pMax(p){}
	Bounds3(const Point3<T>& p1, const Point3<T>& p2) 
		: pMin(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z)),
		  pMax(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z)){}
	template <class U>
	explicit operator Bounds3<U>() const{
		return Bounds3<U>((Point3<U>)pMin, (Point3<U>)pMax);
	}
	const Point3<T>& operator[](size_t i) const{
		if(i!=0 && i!=1)
			throw std::out_of_range("Bounds3 index out of range\n");
		return (i==0 ? pMin : pMax);
	}
	Point3<T>& operator[](size_t i){
		if(i!=0 && i!=1)
			throw std::out_of_range("Bounds3 index out of range\n");
		return (i==0 ? pMin : pMax);
	}
	bool operator==(const Bounds3<T>& b) const{
		return (b.pMin == pMin) && (b.pMax == pMax);
	}
	bool operator!=(const Bounds3<T>& b) const{
		return (b.pMin != pMin) || (b.pMax != pMax);
	}
	Vec3<T> diagnoal() const{
		return pMax - pMin;
	}
	T surfaceArea() const{
		Vec3<T> diag = diagnoal();
		return 2 * (diag.x * diag.y + diag.x * diag.z + diag.y * diag.z);
	}
	T volume() const{
		Vec3<T> diag = diagnoal();
		return (diag.x * diag.y * diag.z);
	}	
	size_t maxExtent() const{
		Vec3<T> diag = diagnoal();
		return (diag.x > diag.y ? (diag.x > diag.z ? 0 : 2) : (diag.y > diag.z ? 1 : 2));
	}
	Vec3<T> offset(const Point3f& p) const{
		Vec3<T> v = p - pMin;
		v.x /= (pMax.x - pMin.x);
		v.y /= (pMax.y - pMin.y);
		v.z /= (pMax.z - pMin.z);
		return v;
	}
	Point3<T> corner(size_t i) const{
		if(i < 0 || i >= 8)
			throw std::out_of_range("Bounds3 index in corner function is out of range!\n");
		return Point3<T>((*this)[(i & 1)].x,
						 (*this)[(i & 2) ? 1 : 0].y,
						 (*this)[(i & 4) ? 1 : 0].z);
	}
	bool isValid(){
		return (pMin.x < pMax.x && pMin.y < pMax.y && pMin.z < pMax.z);
	}
	bool intersectP(const Ray& ray, float* hit_t0 = nullptr, float* hit_t1 = nullptr) const;
	bool intersectP(const Ray& ray, const Vec3f& invDir, const bool isDirNeg[3]) const;

	friend std::ostream& operator<<(std::ostream& os, Bounds3<T> b){
		os << "[ " << b.pMin << ", " << b.pMax << " ]";
		return os;
	}
	
	Point3<T> pMin, pMax;
};

template <class T>
inline bool Bounds3<T>::intersectP(const Ray& ray, float* hit_t0, float* hit_t1) const{
	float t0 = 0, t1 = ray.tMax;
	for(size_t i=0; i<3; ++i){
		if(ray.dir[i] == 0){
			if(ray.o[i] < pMin[i] || ray.o[i] > pMax[i])
				return false;
		}else{
			float invDir = 1.f / ray.dir[i];
			float tNear = (pMin[i] - ray.o[i]) * invDir;
			float tFar  = (pMax[i] - ray.o[i]) * invDir;

			if(tNear > tFar)
				std::swap(tNear, tFar);
			t0 = (tNear > t0) ? tNear : t0;
			t1 = (tFar  < t1) ? tFar  : t1;
			if(t0 > t1) return false;
		}
	}
	if(hit_t0) *hit_t0 = t0;
	if(hit_t1) *hit_t1 = t1;
	return true;
}

template <class T>
bool Bounds3<T>::intersectP(const Ray& ray, const Vec3f& invDir, const bool isDirNeg[3]) const{
	const Bounds3f& b = *this;

	float tMin = (b[isDirNeg[0]].x - ray.o.x) * invDir.x;
	float tMax = (b[1 - isDirNeg[0]].x - ray.o.x) * invDir.x;
	float tyMin = (b[isDirNeg[1]].y - ray.o.y) * invDir.y;
	float tyMax = (b[1 - isDirNeg[1]].y - ray.o.y) * invDir.y;

	if(tMin > tyMax || tyMin > tMax)
		return false;
	tMin = (tMin > tyMin ? tMin : tyMin);
	tMax = (tMax < tyMax ? tMax : tyMax);

	float tzMin = (b[isDirNeg[2]].z - ray.o.z) * invDir.z;
	float tzMax = (b[1 - isDirNeg[2]].z - ray.o.z) * invDir.z;

	if(tMin > tzMax || tzMin > tMax)
		return false;
	tMin = (tMin > tzMin ? tMin : tzMin);
	tMax = (tMax < tzMax ? tMax : tzMax);
	return (tMin < ray.tMax) && (tMax > 0);
}


// geometry inline function
template <class T, class U>
Vec2<T> operator*(U f, const Vec2<T>& v){
	return v * f;
}

template <class T, class U>
Vec3<T> operator*(U f, const Vec3<T>& v){
	return v * f;
}

template <class T, class U>
Normal3<T> operator*(U f, const Normal3<T>& v){
	return v * f;
}

template <class T, class U>
Point2<T> operator*(U f, const Point2<T>& v){
	return v * f;
}

template <class T, class U>
Point3<T> operator*(U f, const Point3<T>& v){
	return v * f;
}

template <class T>
Vec2<T> Abs(const Vec2<T>& v){
	return Vec2<T>(std::abs(v.x), std::abs(v.y));
}

template <class T>
Vec3<T> Abs(const Vec3<T>& v){
	return Vec3<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z));
}

template <class T>
Normal3<T> Abs(const Normal3<T>& v){
	return Normal3<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z));
}

template <class T>
float Dot(const Vec2<T>& v1, const Vec2<T>& v2){
	return v1.x * v2.x + v1.y * v2.y;
}

template <class T>
float Dot(const Vec3<T>& v1, const Vec3<T>& v2){
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template <class T>
float Dot(const Vec3<T>& v1, const Normal3<T>& v2){
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template <class T>
float Dot(const Normal3<T>& v1, const Vec3<T>& v2){
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template <class T>
float Dot(const Normal3<T>& v1, const Normal3<T>& v2){
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template <class T>
float AbsDot(const Vec2<T>& v1, const Vec2<T>& v2){
	return std::abs(Dot(v1, v2));
}

template <class T>
float AbsDot(const Vec3<T>& v1, const Vec3<T>& v2){
	return std::abs(Dot(v1, v2));
}

template <class T>
float AbsDot(const Vec3<T>& v1, const Normal3<T>& v2){
	return std::abs(Dot(v1, v2));
}

template <class T>
float AbsDot(const Normal3<T>& v1, const Vec3<T>& v2){
	return std::abs(Dot(v1, v2));
}

template <class T>
T AbsDot(const Normal3<T>& v1, const Normal3<T>& v2){
	return std::abs(Dot(v1, v2));
}

template <class T>
Vec3<T> Cross(const Vec3<T>& v1, const Vec3<T>& v2){
	return Vec3<T>(
		(v1.y * v2.z) - (v1.z * v2.y),
		(v1.z * v2.x) - (v1.x * v2.z),
		(v1.x * v2.y) - (v1.y * v2.x));
}  

template <class T>
Vec3<T> Cross(const Vec3<T>& v1, const Normal3<T>& v2){
	return Vec3<T>(
		(v1.y * v2.z) - (v1.z * v2.y),
		(v1.z * v2.x) - (v1.x * v2.z),
		(v1.x * v2.y) - (v1.y * v2.x));
} 

template <class T>
Vec3<T> Cross(const Normal3<T>& v1, const Vec3<T>& v2){
	return Vec3<T>(
		(v1.y * v2.z) - (v1.z * v2.y),
		(v1.z * v2.x) - (v1.x * v2.z),
		(v1.x * v2.y) - (v1.y * v2.x));
}

template <class T, template <class U> class Vec >
Vec<T> Normalize(const Vec<T>& v){
	return v / v.length();
}

template <class T>
Vec3<T> Min(const Vec3<T>& v1, const Vec3<T>& v2){
	return Vec3<T>(std::min(v1.x, v2.x), std::min(v1.y, v2.y), std::min(v1.z, v2.z));
}

template <class T>
Vec2<T> Min(const Vec2<T>& v1, const Vec2<T>& v2){
	return Vec2<T>(std::min(v1.x, v2.x), std::min(v1.y, v2.y));
}

template <class T>
Point3<T> Min(const Point3<T>& v1, const Point3<T>& v2){
	return Point3<T>(std::min(v1.x, v2.x), std::min(v1.y, v2.y), std::min(v1.z, v2.z));
}

template <class T>
Point2<T> Min(const Point2<T>& v1, const Point2<T>& v2){
	return Point2<T>(std::min(v1.x, v2.x), std::min(v1.y, v2.y));
}

template <class T>
Vec3<T> Max(const Vec3<T>& v1, const Vec3<T>& v2){
	return Vec3<T>(std::max(v1.x, v2.x), std::max(v1.y, v2.y), std::max(v1.z, v2.z));
}

template <class T>
Vec2<T> Max(const Vec2<T>& v1, const Vec2<T>& v2){
	return Vec2<T>(std::max(v1.x, v2.x), std::max(v1.y, v2.y));
}

template <class T>
Point3<T> Max(const Point3<T>& v1, const Point3<T>& v2){
	return Point3<T>(std::max(v1.x, v2.x), std::max(v1.y, v2.y), std::max(v1.z, v2.z));
}

template <class T>
Point2<T> Max(const Point2<T>& v1, const Point2<T>& v2){
	return Point2<T>(std::max(v1.x, v2.x), std::max(v1.y, v2.y));
}

template <class T, template<class U> class Vec>
Vec<T> Permute(const Vec<T>& v, int x, int y, int z){
	return Vec<T>(v[x], v[y], v[z]);
}

template <class T, template<class U> class Vec>
Vec<T> Permute(const Vec<T>& v, int x, int y){
	return Vec<T>(v[x], v[y]);
}

template <class T>
float Distance(const Point2<T>& p1, const Point2<T>& p2){
	return (p1 - p2).length();
}

template <class T>
float Distance(const Point3<T>& p1, const Point3<T>& p2){
	return (p1 - p2).length();
}

template <class T>
float DistanceSquared(const Point2<T>& p1, const Point2<T>& p2){
	return (p1 - p2).lengthSquared();
}

template <class T>
float DistanceSquared(const Point3<T>& p1, const Point3<T>& p2){
	return (p1 - p2).lengthSquared();
}

template <class T>
Point2<T> Lerp(const Point2<T>& p1, const Point2<T>& p2, float t){
	return p1 * (1.0f - t) + p2 * t;
}

template <class T>
Point3<T> Lerp(const Point3<T>& p1, const Point3<T>& p2, float t){
	return p1 * (1.0f - t) + p2 * t;
}

template <class T>
Point2<T> Floor(const Point2<T>& p){
	return Point2<T>(std::floor(p.x), std::floor(p.y));
}

template <class T>
Point3<T> Floor(const Point3<T>& p){
	return Point3<T>(std::floor(p.x), std::floor(p.y), std::floor(p.z));
}

template <class T>
Point2<T> Ceil(const Point2<T>& p){
	return Point2<T>(std::ceil(p.x), std::ceil(p.y));
}

template <class T>
Point3<T> Ceil(const Point3<T>& p){
	return Point3<T>(std::ceil(p.x), std::ceil(p.y), std::ceil(p.z));
}

template <class T, template <class U> class Bounds>
Bounds<T> Union(const Bounds<T>& b1, const Bounds<T>& b2){
	return Bounds<T>(Min(b1.pMin, b2.pMin), Max(b1.pMax, b2.pMax));
}

template <class T, template <class U> class Bounds, template <class U> class Point>
Bounds<T> Union(const Bounds<T>& b, const Point<T>& p){
	return Bounds<T>(Min(b.pMin, p), Max(b.pMax, p));
}

template <class T, template <class U> class Bounds>
Bounds<T> Intersect(const Bounds<T>& b1, const Bounds<T>& b2){
	return Bounds<T>(Max(b1.pMin, b2.pMin), Min(b1.pMax, b2.pMax));
}

template<typename T>
bool Overlap(const Bounds3<T>& b1, const Bounds3<T>& b2){
	bool x = (b1.pMax.x >= b2.pMin.x) && (b2.pMax.x >= b1.pMin.x);
	bool y = (b1.pMax.y >= b2.pMin.y) && (b2.pMax.y >= b1.pMin.y);
	bool z = (b1.pMax.z >= b2.pMin.z) && (b2.pMax.z >= b1.pMin.z);
	return (x && y && z);
}

template<typename T>
bool Overlap(const Bounds2<T>& b1, const Bounds2<T>& b2){
	bool x = (b1.pMax.x >= b2.pMin.x) && (b2.pMax.x >= b1.pMin.x);
	bool y = (b1.pMax.y >= b2.pMin.y) && (b2.pMax.y >= b1.pMin.y);
	return (x && y);
}


template<typename T>
bool Inside(const Point3<T>& p, const Bounds3<T>& b){
	return (p.x >= b.pMin.x && p.x <= b.pMax.x && 
		    p.y >= b.pMin.y && p.y <= b.pMax.y &&
		    p.z >= b.pMin.z && p.z <= b.pMax.z);
}

template<typename T>
bool Inside(const Point2<T>& p, const Bounds2<T>& b){
	return (p.x >= b.pMin.x && p.x <= b.pMax.x && 
		    p.y >= b.pMin.y && p.y <= b.pMax.y);
}

template<typename T>
bool InsideExclusive(const Point3<T>& p, const Bounds3<T>& b){
	return (p.x >= b.pMin.x && p.x < b.pMax.x && 
		    p.y >= b.pMin.y && p.y < b.pMax.y &&
		    p.z >= b.pMin.z && p.z < b.pMax.z);
}

template<typename T>
bool InsideExclusive(const Point2<T>& p, const Bounds2<T>& b){
	return (p.x >= b.pMin.x && p.x < b.pMax.x && 
		    p.y >= b.pMin.y && p.y < b.pMax.y);
}

template<typename T, typename U>
Bounds3<T> Expand(const Bounds3<T>& b, U delta){
	return Bounds3<T>(b.pMin - Vec3<T>(delta, delta, delta), b.pMax + Vec3<T>(delta, delta, delta));
}

template<typename T, typename U>
Bounds2<T> Expand(const Bounds2<T>& b, U delta){
	return Bounds3<T>(b.pMin - Vec2<T>(delta, delta), b.pMax + Vec2<T>(delta, delta));
}

RIGA_NAMESPACE_END