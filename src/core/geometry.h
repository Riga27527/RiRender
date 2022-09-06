
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
class Normal3;

typedef Vec2<int> Vec2i;
typedef Vec2<float> Vec2f;

typedef Vec3<int> Vec3i;
typedef Vec3<float> Vec3f;

typedef Point2<int> Point2i;
typedef Point2<float> Point2f;

typedef Point3<int> Point3i;
typedef Point3<float> Point3f;

typedef Normal3<float> Normal3f;


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

template <class T>
std::ostream& operator<<(std::ostream& os, const Normal3<T>& v){
	os << "[ " << v.x << ", " << v.y << ", " << v.z << " ]";
	return os;
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

RIGA_NAMESPACE_END