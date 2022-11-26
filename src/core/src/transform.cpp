
#include <core/transform.h>

RIGA_NAMESPACE_BEGIN

Mat4x4f Mul(const Mat4x4f& m1, const Mat4x4f& m2){
	Mat4x4f r;
	for(int i=0; i<4; ++i)
		for(int j=0; j<4; ++j)
			for(int k=0; k<4; ++k)
				r.m[i][j] += m1.m[i][k] * m2.m[k][j];
	return r;
}

Mat4x4f Transpose(const Mat4x4f& mat){
	return Mat4x4f(mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
				mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1],
				mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
				mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3]);
}

std::ostream& operator<<(std::ostream& os, const Mat4x4f& mat){
	os << "[[ " << mat.m[0][0] << ", " << mat.m[0][1] << ", " << mat.m[0][2] << ", " << mat.m[0][3] << " ],\n" 
	   << " [ " << mat.m[1][0] << ", " << mat.m[1][1] << ", " << mat.m[1][2] << ", " << mat.m[1][3] << " ],\n" 
	   << " [ " << mat.m[2][0] << ", " << mat.m[2][1] << ", " << mat.m[2][2] << ", " << mat.m[2][3] << " ],\n" 
	   << " [ " << mat.m[3][0] << ", " << mat.m[3][1] << ", " << mat.m[3][2] << ", " << mat.m[3][3] << " ]]";
	return os;
}

Mat4x4f Inverse(const Mat4x4f& m){
	int indxc[4], indxr[4];
    int ipiv[4] = {0, 0, 0, 0};
    float minv[4][4];
    memcpy(minv, m.m, 4 * 4 * sizeof(float));
    for (int i = 0; i < 4; i++) {
        int irow = 0, icol = 0;
        float big = 0.f;
        // Choose pivot
        for (int j = 0; j < 4; j++) {
            if (ipiv[j] != 1) {
                for (int k = 0; k < 4; k++) {
                    if (ipiv[k] == 0) {
                        if (std::abs(minv[j][k]) >= big) {
                            big = float(std::abs(minv[j][k]));
                            irow = j;
                            icol = k;
                        }
                    } else if (ipiv[k] > 1)
                        throw("Singular matrix in MatrixInvert");
                }
            }
        }
        ++ipiv[icol];
        // Swap rows _irow_ and _icol_ for pivot
        if (irow != icol) {
            for (int k = 0; k < 4; ++k) std::swap(minv[irow][k], minv[icol][k]);
        }
        indxr[i] = irow;
        indxc[i] = icol;
        if (minv[icol][icol] == 0.f) throw("Singular matrix in MatrixInvert");

        // Set $m[icol][icol]$ to one by scaling row _icol_ appropriately
        float pivinv = 1. / minv[icol][icol];
        minv[icol][icol] = 1.;
        for (int j = 0; j < 4; j++) minv[icol][j] *= pivinv;

        // Subtract this row from others to zero out their columns
        for (int j = 0; j < 4; j++) {
            if (j != icol) {
                float save = minv[j][icol];
                minv[j][icol] = 0;
                for (int k = 0; k < 4; k++) minv[j][k] -= minv[icol][k] * save;
            }
        }
    }
    // Swap columns to reflect permutation
    for (int j = 3; j >= 0; j--) {
        if (indxr[j] != indxc[j]) {
            for (int k = 0; k < 4; k++)
                std::swap(minv[k][indxr[j]], minv[k][indxc[j]]);
        }
    }
    return Mat4x4f(minv);
}

std::ostream& operator<<(std::ostream& os, const Transform& t){
	os << "t = \n" << t.m << ", \ninv = \n" << t.mInv;
	return os;
}

Transform Translate(const Vec3f& v){
	Mat4x4f m(1.f, 0.f, 0.f, v.x, 
			  0.f, 1.f, 0.f, v.y,
			  0.f, 0.f, 1.f, v.z,
			  0.f, 0.f, 0.f, 1.f);
	Mat4x4f mInv(1.f, 0.f, 0.f, -v.x, 
			  	 0.f, 1.f, 0.f, -v.y,
			  	 0.f, 0.f, 1.f, -v.z,
			  	 0.f, 0.f, 0.f, 1.f);
	return Transform(m, mInv);
}

Transform Scale(const Vec3f& v){
	Mat4x4f m(v.x, 0.f, 0.f, 0.f,
			  0.f, v.y, 0.f, 0.f,
			  0.f, 0.f, v.z, 0.f,
			  0.f, 0.f, 0.f, 1.f);
	Mat4x4f mInv(1.f / v.x, 0.f, 		0.f, 		0.f,
			  	 0.f, 		1.f / v.y, 	0.f, 		0.f,
			  	 0.f, 		0.f, 		1.f / v.z, 	0.f,
			  	 0.f, 		0.f, 		0.f, 		1.f);	
	return Transform(m, mInv);
}

Transform Scale(float x, float y, float z){
	return Scale(Vec3f(x, y, z));
}

Transform RotateX(float theta){
	float cosTheta = std::cos(Radians(theta));
	float sinTheta = std::sin(Radians(theta));
	Mat4x4f m(1.f, 		 0.f,  0.f, 	  0.f,
			  0.f,  cosTheta,  -sinTheta, 0.f,
			  0.f,  sinTheta,  cosTheta,  0.f,
			  0.f,		 0.f,  0.f, 	  1.f);
	return Transform(m, Transpose(m));
}

Transform RotateY(float theta){
	float cosTheta = std::cos(Radians(theta));
	float sinTheta = std::sin(Radians(theta));
	Mat4x4f m(cosTheta,  0.f,  sinTheta, 0.f,
			  0.f, 		 1.f,  0.f, 	 0.f,
			  -sinTheta, 0.f,  cosTheta, 0.f,
			  0.f,		 0.f,  0.f, 	 1.f);
	return Transform(m, Transpose(m));
}

Transform RotateZ(float theta){
	float cosTheta = std::cos(Radians(theta));
	float sinTheta = std::sin(Radians(theta));
	Mat4x4f m(cosTheta,  -sinTheta, 0.f,   0.f,
			  sinTheta,	  cosTheta, 0.f,   0.f,
			  0.f, 		 0.f,  		1.f,   0.f,
			  0.f,		 0.f,  		0.f,   1.f);
	return Transform(m, Transpose(m));
}

Transform Rotate(float theta, const Vec3f& axis){
    Vec3f a = Normalize(axis);
    float sinTheta = std::sin(Radians(theta));
    float cosTheta = std::cos(Radians(theta));
    Mat4x4f m;
    // Compute rotation of first basis vector
    m[0][0] = a.x * a.x + (1 - a.x * a.x) * cosTheta;
    m[0][1] = a.x * a.y * (1 - cosTheta) - a.z * sinTheta;
    m[0][2] = a.x * a.z * (1 - cosTheta) + a.y * sinTheta;
    m[0][3] = 0;

    // Compute rotations of second and third basis vectors
    m[1][0] = a.x * a.y * (1 - cosTheta) + a.z * sinTheta;
    m[1][1] = a.y * a.y + (1 - a.y * a.y) * cosTheta;
    m[1][2] = a.y * a.z * (1 - cosTheta) - a.x * sinTheta;
    m[1][3] = 0;

    m[2][0] = a.x * a.z * (1 - cosTheta) - a.y * sinTheta;
    m[2][1] = a.y * a.z * (1 - cosTheta) + a.x * sinTheta;
    m[2][2] = a.z * a.z + (1 - a.z * a.z) * cosTheta;
    m[2][3] = 0;
    return Transform(m, Transpose(m));
}

Transform LookAt(const Point3f& pos, const Point3f& focus, const Vec3f& up){
	Mat4x4f camera2World;
	camera2World[0][3] = pos.x;
	camera2World[1][3] = pos.y;
	camera2World[2][3] = pos.z;
	camera2World[3][3] = 1.f;

	Vec3f dir = Normalize(focus - pos);
	Vec3f right = Cross(Normalize(up), dir);
	if(right.lengthSquared() == 0)
		throw("up vector and view vector pass to LookAt function are in the same direction!!!");
	else
		right.normalized();
	Vec3f newUp = Normalize(Cross(dir, right));

	camera2World[0][0] = right.x;
	camera2World[1][0] = right.y;
	camera2World[2][0] = right.z;

	camera2World[0][1] = newUp.x;
	camera2World[1][1] = newUp.y;
	camera2World[2][1] = newUp.z;
	
	camera2World[0][2] = dir.x;
	camera2World[1][2] = dir.y;
	camera2World[2][2] = dir.z;

	return Transform(Inverse(camera2World), camera2World);
}

Transform Orthographic(float znear, float zfar){
	return Scale(1.f, 1.f, 1.f / (zfar - znear)) * Translate(Vec3f(0.f, 0.f, -znear));
}

Transform Perspective(float fov, float znear, float zfar){
	float A = zfar / (zfar - znear), B = -zfar * znear / (zfar - znear);
	Mat4x4f persp(1.f, 0.f, 0.f, 0.f,
				  0.f, 1.f, 0.f, 0.f,
				  0.f, 0.f, A, 	 B,
				  0.f, 0.f, 1.f, 0.f);

	float invTan = 1.f / std::tan(Radians(fov) * 0.5f);
	return Scale(invTan, invTan, 1.f) * Transform(persp);
}

RIGA_NAMESPACE_END