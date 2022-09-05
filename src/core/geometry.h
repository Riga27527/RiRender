
#pragma once

#include "common.h"

RIGA_NAMESPACE_BEGIN

template <class T>
class Vec2;
template <class T>
class Vec3;
template <class T>
class Point2;
template <class T>
class Point3;

template <class T>
class Vec2{
public:
	Vec2(){}
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
	
	float LengthSquared() const {return x*x + y*y; }
	float Length() const {return std::sqrt(LengthSquared()); }	
	bool hasNaN() const {return (isNaN(x) || isNaN(y)); }
	Vec2<T>& normalized(){
		float inv = 1.0f / Length();
		x *= inv;
		y *= inv;
		return *this;
	}

	// public data
	T x = 0 , y = 0;
};

template <class T>
std::ostream& operator<<(std::ostream& os, const Vec2<T>& v){
	os << "[ " << v.x << ", " << v.y << " ]";
	return os;
}

template <class T>
class Vec3{
public:
	Vec3(){}
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
	
	float LengthSquared() const {return x*x + y*y + z*z; }
	float Length() const {return std::sqrt(LengthSquared()); }	
	bool hasNaN() const {return (isNaN(x) || isNaN(y) || isNaN(z)); }
	Vec3<T>& normalized(){
		float inv = 1.0f / Length();
		x *= inv;
		y *= inv;
		z *= inv;
		return *this;
	}

	T x = 0, y = 0, z = 0;
};

template <class T>
std::ostream& operator<<(std::ostream& os, const Vec3<T>& v){
	os << "[ " << v.x << ", " << v.y << ", " << v.z << " ]";
	return os;
}

template <class T>
class Point2{
public:
	Point2(){}
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

	bool hasNaN() const {return (isNaN(x) || isNaN(y)); }

	// public data
	T x = 0 , y = 0;
};

template <class T>
std::ostream& operator<<(std::ostream& os, const Point2<T>& v){
	os << "[ " << v.x << ", " << v.y << " ]";
	return os;
}

template <class T>
class Point3{
public:
	Point3(){}
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
		return Point2<T>(x+p.x, y+p.y, z+p.z);
	}

	// point + vec = point
	Point3<T>& operator+=(const Vec3<T>& vec){
		x += vec.x;
		y += vec.y;
		return *this;
	}

	Point3<T> operator+(const Vec3<T>& vec){
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

	Point3<T> operator-(const Vec3<T>& vec){
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

	bool hasNaN() const {return (isNaN(x) || isNaN(y) || isNaN(z)); }

	// public data
	T x = 0 , y = 0, z = 0;
};

template <class T>
std::ostream& operator<<(std::ostream& os, const Point3<T>& v){
	os << "[ " << v.x << ", " << v.y << ", " << v.z << " ]";
	return os;
}

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
	
	float LengthSquared() const {return x*x + y*y + z*z; }
	float Length() const {return std::sqrt(LengthSquared()); }	
	bool hasNaN() const {return (isNaN(x) || isNaN(y) || isNaN(z)); }
	Normal3<T>& normalized(){
		float inv = 1.0f / Length();
		x *= inv;
		y *= inv;
		z *= inv;
		return *this;
	}

	T x = 0, y = 0, z = 0;
};

template <class T>
std::ostream& operator<<(std::ostream& os, const Normal3<T>& v){
	os << "[ " << v.x << ", " << v.y << ", " << v.z << " ]";
	return os;
}

RIGA_NAMESPACE_END