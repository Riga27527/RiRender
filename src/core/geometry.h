
#pragma once

#include "core/common.h"

RIGA_NAMESPACE_BEGIN

template <class T>
class Vec2{
public:
	Vec2(){}
	Vec2(T xx, T yy) : x(xx), y(yy){}
	Vec2(const Vec2<T>& vec2) : x(vec2.x), y(vec2.y){}
	Vec2<T>& operator=(const Vec2<T> &vec2) : x(vec2.x), y(vec2.y){
		return *this;
	}

	Vec2<T>& operator+=(const Vec2<T> &vec2){
		x += vec2.x;
		y += vec2.y;
		return *this;
	}

	Vec2<T> operator+(const Vec2<T> &vec2) const{
		return Vec2<T>(x+vec2.x, y+vec2.y);
	}

	Vec2<T>& operator-=(const Vec2<T> &vec2){
		x -= vec2.x;
		y -= vec2.y;
		return *this;
	}

	Vec2<T> operator-(const Vec2<T> &vec2) const{
		return Vec2<T>(x-vec2.x, y-vec.y);
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
		x /= f;
		y /= f;
		return *this;
	}

	template <class U>
	Vec2<T> operator/(U f) const{
		return Vec2(x/f, y/f);
	}

	bool operator==(const Vec2<T> &vec2) const{
		return (x == vec2.x && y == vec2.y);
	}
	bool operator!=(const Vec2<T> &vec2) const{
		return (x != vec2.x || y != vec2.y);
	}

	T operator[](size_t i) const{
		if(i!=0 && i!=1){
			throw std::out_of_range("Vec2 index out of range\n")
		}
		return i==0 ? x : y;
	}

	T& operator[](size_t i){
		if(i!=0 && i!=1){
			throw std::out_of_range("Vec2 index out of range\n")
		}
		return i==0 ? x : y;
	}
	
	float LengthSquared() const {return x*x + y*y; }
	float Length() const {return std::sqrt(LengthSquared()); }	
	bool hasNaN() const {return (isNaN(x) || isNaN(y)); }

	// public data
	T x = 0 , y = 0;
};

RIGA_NAMESPACE_END