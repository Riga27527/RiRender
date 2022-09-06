
#pragma once

#include "geometry.h"
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
	static Mat4x4f identity(){
		Mat4x4f mat;
        mat.m[0][0] = mat.m[1][1] = mat.m[2][2] = mat.m[3][3] = 1.f;
        mat.m[0][1] = mat.m[0][2] = mat.m[0][3] = 
        mat.m[1][0] = mat.m[1][2] = mat.m[1][3] = 
        mat.m[2][0] = mat.m[2][1] = mat.m[2][3] = 
        mat.m[3][0] = mat.m[3][1] = mat.m[3][2] = 0.f;
        return mat;
	}
	float m[4][4] = {0};
};

Mat4x4f Transpose(const Mat4x4f& mat);
Mat4x4f Inverse(const Mat4x4f& m);
Mat4x4f Mul(const Mat4x4f& m1, const Mat4x4f& m2);
std::ostream& operator<<(std::ostream& os, const Mat4x4f& mat);

RIGA_NAMESPACE_END