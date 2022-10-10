#pragma once

#include "geometry.h"

RIGA_NAMESPACE_BEGIN

enum class SpectrumType
{
	Reflectance,
	Illuminant
};

inline void XYZ2RGB(const float xyz[3], float rgb[3]) {
    rgb[0] = 3.240479f * xyz[0] - 1.537150f * xyz[1] - 0.498535f * xyz[2];
    rgb[1] = -0.969256f * xyz[0] + 1.875991f * xyz[1] + 0.041556f * xyz[2];
    rgb[2] = 0.055648f * xyz[0] - 0.204043f * xyz[1] + 1.057311f * xyz[2];
}

inline void RGB2XYZ(const float rgb[3], float xyz[3]) {
    xyz[0] = 0.412453f * rgb[0] + 0.357580f * rgb[1] + 0.180423f * rgb[2];
    xyz[1] = 0.212671f * rgb[0] + 0.715160f * rgb[1] + 0.072169f * rgb[2];
    xyz[2] = 0.019334f * rgb[0] + 0.119193f * rgb[1] + 0.950227f * rgb[2];
}

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
	CoefficientSpectrum& operator*=(float a){
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
	CoefficientSpectrum& operator/=(float a){
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
			throw("out of range in CoefficientSpectrum!");
		return c[i];
	}
	float operator[](int i) const{
		if(i < 0 || i > nSamples)
			throw("out of range in CoefficientSpectrum!");
		return c[i];
	}
	std::string toString() const{
		std::string str = "[ ";
		for(size_t i=0; i < nSamples; ++i){
			str += std::to_string(c[i]);
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

class RGBSpectrum : public CoefficientSpectrum<3>{
	using CoefficientSpectrum<3>::c;

public:
	RGBSpectrum(float v = 0.f) : CoefficientSpectrum<3>(v){}
	RGBSpectrum(const CoefficientSpectrum<3>& v) : CoefficientSpectrum<3>(v){}
	RGBSpectrum(const RGBSpectrum& s, SpectrumType type = SpectrumType::Reflectance){
		*this = s;
	}
	static RGBSpectrum fromRGB(const float rgb[3], SpectrumType type = SpectrumType::Reflectance){
		RGBSpectrum s;
		s.c[0] = rgb[0];
		s.c[1] = rgb[1];
		s.c[2] = rgb[2];
		return s;
	}
	void toRGB(float* rgb) const{
		rgb[0] = c[0];
		rgb[1] = c[1];
		rgb[2] = c[2];
	}
	float toY() const{
		const float yWeights[3] = {0.212671f, 0.715160f, 0.072169f};
		return yWeights[0] * c[0] + yWeights[1] * c[1] + yWeights[2] * c[2];
	}
	static RGBSpectrum fromXYZ(const float xyz[3], SpectrumType type = SpectrumType::Reflectance){
		RGBSpectrum s;
		XYZ2RGB(xyz, s.c);
		return s;
	}
	void toXYZ(float xyz[3]) const{
		RGB2XYZ(c, xyz);
	}
};

inline RGBSpectrum Lerp(const RGBSpectrum& r1, const RGBSpectrum& r2, float t){
	return r1 * (1.f - t) + r2 * t;
}

RIGA_NAMESPACE_END