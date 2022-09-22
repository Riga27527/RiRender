#pragma once

#include "geometry.h"

RIGA_NAMESPACE_BEGIN
template<int nSamples>
class CoefficientSpectrum{
public:
	CoefficientSpectrum(float v = 0.f){
		for(size_t i=0; i<nSamples; ++i)
			c[i] = v;
	}
	CoefficientSpectrum(const CoefficientSpectrum& s){
		for(size_t i=0; i<nSamples; ++i)
			c[i] = s.c[i];
	}
	CoefficientSpectrum& operator=(const CoefficientSpectrum& s){
		for(size_t i=0; i<nSamples; ++i)
			c[i] = s.c[i];
		return *this;
	}
	CoefficientSpectrum& operator+=(const CoefficientSpectrum& s){
		for(size_t i=0; i<nSamples; ++i)
			c[i] += s.c[i];
		return *this;
	}	
	CoefficientSpectrum& operator-=(const CoefficientSpectrum& s){
		for(size_t i=0; i<nSamples; ++i)
			c[i] -= s.c[i];
		return *this;
	}	
	CoefficientSpectrum& operator*=(const CoefficientSpectrum& s){
		for(size_t i=0; i<nSamples; ++i)
			c[i] *= s.c[i];
		return *this;
	}
	CoefficientSpectrum& operator/=(const CoefficientSpectrum& s){
		for(size_t i=0; i<nSamples; ++i){
			if(s.c[i] == 0.f)
				throw("divide zero in CoefficientSpectrum!");
			c[i] /= s.c[i];
		}
		return *this;
	}		
	CoefficientSpectrum operator+(const CoefficientSpectrum& s) const{
		CoefficientSpectrum ret = *this;
		for(size_t i=0; i<nSamples; ++i)
			ret.c[i] += s.c[i];
		return ret;
	}
	CoefficientSpectrum operator-(const CoefficientSpectrum& s) const{
		CoefficientSpectrum ret = *this;
		for(size_t i=0; i<nSamples; ++i)
			ret.c[i] -= s.c[i];
		return ret;
	}
	CoefficientSpectrum operator*(const CoefficientSpectrum& s) const{
		CoefficientSpectrum ret = *this;
		for(size_t i=0; i<nSamples; ++i)
			ret.c[i] *= s.c[i];
		return ret;
	}
	CoefficientSpectrum operator/(const CoefficientSpectrum& s) const{
		CoefficientSpectrum ret = *this;
		for(size_t i=0; i<nSamples; ++i){
			if(s.c[i] == 0.f)
				throw("divide zero in CoefficientSpectrum!");
			ret.c[i] /= s.c[i];
		}
		return ret;
	}
	CoefficientSpectrum operator*(float a) const{
		CoefficientSpectrum ret = *this;
		for(size_t i=0; i<nSamples; ++i)
			ret.c[i] *= a;
		return ret;		
	}
	CoefficientSpectrum& operator*=(float a) const{
		for(size_t i=0; i<nSamples; ++i)
			c[i] *= a;
		return *this;		
	}
	CoefficientSpectrum operator/(float a) const{
		if(a == 0.f)
			throw("divide zero in CoefficientSpectrum!");
		CoefficientSpectrum ret = *this;
		for(size_t i=0; i<nSamples; ++i)			
			ret.c[i] /= a;
		return ret;		
	}
	CoefficientSpectrum& operator/=(float a) const{
		if(a == 0.f)
			throw("divide zero in CoefficientSpectrum!");	
		for(size_t i=0; i<nSamples; ++i)
			c[i] /= a;
		return *this;		
	}
	bool operator==(const CoefficientSpectrum& s) const{
		for(size_t i=0; i<nSamples; ++i)
			if(c[i] != s.c[i])
				return false;
		return true;
	}
	bool operator!=(const CoefficientSpectrum& s) const{
		for(size_t i=0; i<nSamples; ++i)
			if(c[i] != s.c[i])
				return true;
		return false;		
	}
	bool isBlack() const{
		for(size_t i=0; i<nSamples; ++i)
			if(c[i] != 0.f)
				return false;
		return true;
	}
	float maxComponentValue() const{
		float m = c[0];
		for(size_t i=1; i<nSamples; ++i)
			m = std::max(m, c[i]);
		return m;
	}
	bool hasNaN() const{
		for(size_t i=0; i< nSamples; ++i)
			if(isNaN(c[i]))
				return true;
		return false;
	}
	float& operator[](int i){
		if(i < 0 || i > nSamples)
			throw("out of range in CoefficientSpectrum!")
		return c[i];
	}
	float operator[](int i) const{
		if(i < 0 || i > nSamples)
			throw("out of range in CoefficientSpectrum!")
		return c[i];
	}
	std::string toString() const{
		std::string str = "[ ";
		for(size_t i=0; i < nSamples; ++i){
			str += std::toString(c[i]);
			if(i+1 < nSamples)
				str += ", ";
		}
		str += " ]";
		return str;
	}
	friend std::ostream& operator<<(std::ostream& os, const CoefficientSpectrum& s){
		os << s.toString();
		return os;
	}
	friend CoefficientSpectrum operator*(float a, const CoefficientSpectrum& s){
		return s * a;
	}
	friend CoefficientSpectrum Clamp(const CoefficientSpectrum& s, float small, float large){
		CoefficientSpectrum ret;
		for(size_t i=0; i<nSamples; ++i)
			ret.c[i] = Clamp(s.c[i], small, large);
		return ret;
	}
	friend CoefficientSpectrum Sqrt(const CoefficientSpectrum& s){
		CoefficientSpectrum ret;
		for(size_t i=0; i<nSamples; ++i)
			ret.c[i] = std::sqrt(s.c[i]);
		return ret;
	}
	friend CoefficientSpectrum Pow(const CoefficientSpectrum& s, float e){
		CoefficientSpectrum ret;
		for(size_t i=0; i<nSamples; ++i)
			ret.c[i] = std::pow(s.c[i], e);
		return ret;
	}
	friend CoefficientSpectrum Exp(const CoefficientSpectrum& s){
		CoefficientSpectrum ret;
		for(size_t i=0; i<nSamples; ++i)
			ret.c[i] = std::exp(s.c[i]);
		return ret;
	}	
protected:
	float c[nSamples];
};


RIGA_NAMESPACE_END