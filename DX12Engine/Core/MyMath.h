#pragma once

#include <algorithm>
#include <cmath>

#define USE_MATH_DEFINES
#define SQ(x) (x * x)

#undef min
#undef max

/* Templated Linear Interpolation */
template<typename Type>
static Type lerp(const Type a, const Type b, float t) {
	return (a * (1.f - t)) + (b * t);
}

/* Vec3 Class */
class Vec3 {
public:
	union {
		struct { float x, y, z; };
		float v[3];
	};

	Vec3(float _x = 0.f, float _y = 0.f, float _z = 0.f) : x(_x), y(_y), z(_z) {}

	// Operator Overriding
	Vec3 operator+(const Vec3& pVec) const { return Vec3(v[0] + pVec.v[0], v[1] + pVec.v[1], v[2] + pVec.v[2]); }
	Vec3 operator-(const Vec3& pVec) const { return Vec3(v[0] - pVec.v[0], v[1] - pVec.v[1], v[2] - pVec.v[2]); }
	Vec3 operator*(const Vec3& pVec) const { return Vec3(v[0] * pVec.v[0], v[1] * pVec.v[1], v[2] * pVec.v[2]); }
	Vec3 operator/(const Vec3& pVec) const { return Vec3(v[0] / pVec.v[0], v[1] / pVec.v[1], v[2] / pVec.v[2]); }

	Vec3& operator+=(const Vec3& pVec) { v[0] += pVec.v[0]; v[1] += pVec.v[1]; v[2] += pVec.v[2]; return *this; }
	Vec3& operator-=(const Vec3& pVec) { v[0] -= pVec.v[0]; v[1] -= pVec.v[1]; v[2] -= pVec.v[2]; return *this; }
	Vec3& operator*=(const Vec3& pVec) { v[0] *= pVec.v[0]; v[1] *= pVec.v[1]; v[2] *= pVec.v[2]; return *this; }
	Vec3& operator/=(const Vec3& pVec) { v[0] /= pVec.v[0]; v[1] /= pVec.v[1]; v[2] /= pVec.v[2]; return *this; }

	Vec3 operator*(const float s) const { return Vec3(v[0] * s, v[1] * s, v[2] * s); }
	Vec3 operator/(const float s) const { float invS = 1.f / s; return Vec3(v[0] * invS, v[1] * invS, v[2] * invS); }

	Vec3& operator*=(const float s) { v[0] *= s; v[1] *= s; v[2] *= s; return *this; }
	Vec3& operator/=(const float s) { float invS = 1.f / s; v[0] *= invS; v[1] *= invS; v[2] *= invS; return *this; }

	Vec3 operator-() const { return Vec3(-v[0], -v[1], -v[2]); }

	// Methods
	float length() const { return sqrtf(SQ(x) + SQ(y) + SQ(z)); }
	float lengthSquared() const { return SQ(x) + SQ(y) + SQ(z); }

	Vec3 normalize() const { float l = 1.f / sqrtf(SQ(x) + SQ(y) + SQ(z)); return Vec3(x * l, y * l, z * l); }
	float normalizeAndGetLength() { float l = sqrtf(SQ(x) + SQ(y) + SQ(z)); float invL = 1.f / l; x *= invL; y *= invL; z *= invL; return l; }
};
static float Dot(const Vec3& v1, const Vec3& v2) { return ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z)); }
static Vec3 Cross(const Vec3& v1, const Vec3& v2) { return Vec3((v1.y * v2.z) - (v1.z * v2.y), (v1.z * v2.x) - (v1.x * v2.z), (v1.x * v2.y) - (v1.y * v2.x)); }
static Vec3 Min(const Vec3& v1, const Vec3& v2) { return Vec3(std::min(v1.x, v2.x), std::min(v1.y, v2.y), std::min(v1.z, v2.z)); }
static Vec3 Max(const Vec3& v1, const Vec3& v2) { return Vec3(std::max(v1.x, v2.x), std::max(v1.y, v2.y), std::max(v1.z, v2.z)); }

/* Vec4 Class (Homogenious Coordinates) */
class Vec4 {
public:
	union {
		struct { float x, y, z, w; };
		float v[4];
	};

	Vec4(float _x = 0.f, float _y = 0.f, float _z = 0.f, float _w = 1.f) : x(_x), y(_y), z(_z), w(_w) {}

	// Operator Overriding
	Vec4 operator+(const Vec4& pVec) const { return Vec4(v[0] + pVec.v[0], v[1] + pVec.v[1], v[2] + pVec.v[2], 0.f); }
	Vec4 operator-(const Vec4& pVec) const { return Vec4(v[0] - pVec.v[0], v[1] - pVec.v[1], v[2] - pVec.v[2], 0.f); }

	Vec4& operator+=(const Vec4& pVec) { v[0] += pVec.v[0]; v[1] += pVec.v[1]; v[2] += pVec.v[2]; v[3] = 0.f;  return *this; }
	Vec4& operator-=(const Vec4& pVec) { v[0] -= pVec.v[0]; v[1] -= pVec.v[1]; v[2] -= pVec.v[2]; v[3] = 0.f; return *this; }

	Vec4 operator*(const float s) const { return Vec4(v[0] * s, v[1] * s, v[2] * s, v[3] * s); }
	Vec4 operator/(const float s) const { float invS = 1.f / s; return Vec4(v[0] * invS, v[1] * invS, v[2] * invS, v[3] * invS); }

	Vec4& operator*=(const float s) { v[0] *= s; v[1] *= s; v[2] *= s; v[3] *= s; return *this; }
	Vec4& operator/=(const float s) { float invS = 1.f / s; v[0] *= invS; v[1] *= invS; v[2] *= invS, v[3] *= invS; return *this; }

	// Methods
	float length() const { return sqrtf(SQ(x) + SQ(y) + SQ(z)); }
	float lengthSquared() const { return SQ(x) + SQ(y) + SQ(z); }
	void divideW() const { float invW = 1.f / w; x* invW; y* invW; z* invW; }
	void normalize() { float l = 1.f / sqrtf(SQ(x) + SQ(y) + SQ(z)); x *= l; y *= l; z*= l; }
	float normalizeAndGetLength() { float l = sqrtf(SQ(x) + SQ(y) + SQ(z)); float invL = 1.f / l; x *= invL; y *= invL; z *= invL; return l; }
};
static float Dot(const Vec4& v1, const Vec4& v2) { return ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z)); }
static Vec4 Cross(const Vec4& v1, const Vec4& v2) { return Vec4((v1.y * v2.z) - (v1.z * v2.y), (v1.z * v2.x) - (v1.x * v2.z), (v1.x * v2.y) - (v1.y * v2.x), 0.f); }
static Vec4 Min(const Vec4& v1, const Vec4& v2) { return Vec4(std::min(v1.x, v2.x), std::min(v1.y, v2.y), std::min(v1.z, v2.z), std::min(v1.w, v2.w)); }
static Vec4 Max(const Vec4& v1, const Vec4& v2) { return Vec4(std::max(v1.x, v2.x), std::max(v1.y, v2.y), std::max(v1.z, v2.z), std::max(v1.w, v2.w)); }

/* Matrix Class */
class Matrix {
private:
	void identity() {
		memset(a, 0, 16 * sizeof(a[0]));
		a[0] = 1.f; a[5] = 1.f; a[10] = 1.f; a[15] = 1.f;
 	}
public:
	union {
		float m[4][4];
		float a[16];
	};

	Matrix() { identity(); }
	Matrix(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13,
		   float m20, float m21, float m22, float m23, float m30, float m31, float m32, float m33) {
		m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
		m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
		m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
		m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
	}

	float& operator[](int index) { return a[index]; }

	Matrix transpose() {
		return Matrix(m[0][0], m[1][0], m[2][0], m[3][0],
					  m[0][1], m[1][1], m[2][1], m[3][1],
					  m[0][2], m[1][2], m[2][2], m[3][2],
					  m[0][3], m[1][3], m[2][3], m[3][3]);
	}

	static Matrix translate(const Vec3& v) {
		Matrix mat;
		mat.a[3] = v.x; mat.a[7] = v.y; mat.a[11] = v.z;
		return mat;
	}

	static Matrix scale(float s) {
		Matrix mat;
		s = std::min(s, 0.01f);
		mat.a[0] = s; mat.a[5] = s; mat.a[10] = s;
		return mat;
	}

	static Matrix rotateX(float theta) {
		Matrix mat;

		float sint = sinf(theta);
		float cost = cosf(theta);

		mat.a[5] = cost;
		mat.a[6] = sint;
		mat.a[9] = -sint;
		mat.a[10] = cost;

		return mat;
	}

	static Matrix rotateY(float theta) {
		Matrix mat;

		float sint = sinf(theta);
		float cost = cosf(theta);

		mat.a[0] = cost;
		mat.a[2] = -sint;
		mat.a[8] = sint;
		mat.a[10] = cost;

		return mat;
	}

	static Matrix rotateZ(float theta) {
		Matrix mat;

		float sint = sinf(theta);
		float cost = cosf(theta);

		mat.a[0] = cost;
		mat.a[1] = sint;
		mat.a[4] = -sint;
		mat.a[5] = cost;

		return mat;
	}

	static Matrix rotateXYZ(float x, float y, float z) {
		return Matrix::rotateX(x) * Matrix::rotateY(y) * Matrix::rotateZ(z);
	}

	Vec4 mul(const Vec4& v) {
		return Vec4(v.x * a[0] + v.y * a[1] + v.z * a[2] + v.w + a[3],
					v.x * a[4] + v.y * a[5] + v.z * a[6] + v.w + a[7],
					v.x * a[8] + v.y * a[9] + v.z * a[10] + v.w + a[11],
					v.x * a[12] + v.y * a[13] + v.z * a[14] + v.w + a[15]);
	}

	Vec3 mulPoint(const Vec3& v) {
		return Vec3((v.x * a[0] + v.y * a[1] + v.z * a[2]) + a[3],
					(v.x * a[4] + v.y * a[5] + v.z * a[6]) + a[7],
					(v.x * a[8] + v.y * a[9] + v.z * a[10]) + a[11]);
	}

	Matrix mul(const Matrix& matrix) const {
		Matrix res;
		res[0] = a[0] * matrix.a[0] + a[4] * matrix.a[1] + a[8] * matrix.a[2] + a[12] * matrix.a[3];
		res[1] = a[1] * matrix.a[0] + a[5] * matrix.a[1] + a[9] * matrix.a[2] + a[13] * matrix.a[3];
		res[2] = a[2] * matrix.a[0] + a[6] * matrix.a[1] + a[10] * matrix.a[2] + a[14] * matrix.a[3];
		res[3] = a[3] * matrix.a[0] + a[7] * matrix.a[1] + a[11] * matrix.a[2] + a[15] * matrix.a[3];

		res[4] = a[0] * matrix.a[4] + a[4] * matrix.a[5] + a[8] * matrix.a[6] + a[12] * matrix.a[7];
		res[5] = a[1] * matrix.a[4] + a[5] * matrix.a[5] + a[9] * matrix.a[6] + a[13] * matrix.a[7];
		res[6] = a[2] * matrix.a[4] + a[6] * matrix.a[5] + a[10] * matrix.a[6] + a[14] * matrix.a[7];
		res[7] = a[3] * matrix.a[4] + a[7] * matrix.a[5] + a[11] * matrix.a[6] + a[15] * matrix.a[7];

		res[8] = a[0] * matrix.a[8] + a[4] * matrix.a[9] + a[8] * matrix.a[10] + a[12] * matrix.a[11];
		res[9] = a[1] * matrix.a[8] + a[5] * matrix.a[9] + a[9] * matrix.a[10] + a[13] * matrix.a[11];
		res[10] = a[2] * matrix.a[8] + a[6] * matrix.a[9] + a[10] * matrix.a[10] + a[14] * matrix.a[11];
		res[11] = a[3] * matrix.a[8] + a[7] * matrix.a[9] + a[11] * matrix.a[10] + a[15] * matrix.a[11];

		res[12] = a[0] * matrix.a[12] + a[4] * matrix.a[13] + a[8] * matrix.a[14] + a[12] * matrix.a[15];
		res[13] = a[1] * matrix.a[12] + a[5] * matrix.a[13] + a[9] * matrix.a[14] + a[13] * matrix.a[15];
		res[14] = a[2] * matrix.a[12] + a[6] * matrix.a[13] + a[10] * matrix.a[14] + a[14] * matrix.a[15];
		res[15] = a[3] * matrix.a[12] + a[7] * matrix.a[13] + a[11] * matrix.a[14] + a[15] * matrix.a[15];
		return res;
	}

	Matrix operator*(const Matrix& matrix) { return mul(matrix); }

	Matrix invert() {
		Matrix inv;
		inv[0] = a[5] * a[10] * a[15] - a[5] * a[11] * a[14] - a[9] * a[6] * a[15] + a[9] * a[7] * a[14] + a[13] * a[6] * a[11] - a[13] * a[7] * a[10];
		inv[4] = -a[4] * a[10] * a[15] + a[4] * a[11] * a[14] + a[8] * a[6] * a[15] - a[8] * a[7] * a[14] - a[12] * a[6] * a[11] + a[12] * a[7] * a[10];
		inv[8] = a[4] * a[9] * a[15] - a[4] * a[11] * a[13] - a[8] * a[5] * a[15] + a[8] * a[7] * a[13] + a[12] * a[5] * a[11] - a[12] * a[7] * a[9];
		inv[12] = -a[4] * a[9] * a[14] + a[4] * a[10] * a[13] + a[8] * a[5] * a[14] - a[8] * a[6] * a[13] - a[12] * a[5] * a[10] + a[12] * a[6] * a[9];
		
		inv[1] = -a[1] * a[10] * a[15] + a[1] * a[11] * a[14] + a[9] * a[2] * a[15] - a[9] * a[3] * a[14] - a[13] * a[2] * a[11] + a[13] * a[3] * a[10];
		inv[5] = a[0] * a[10] * a[15] - a[0] * a[11] * a[14] - a[8] * a[2] * a[15] + a[8] * a[3] * a[14] + a[12] * a[2] * a[11] - a[12] * a[3] * a[10];
		inv[9] = -a[0] * a[9] * a[15] + a[0] * a[11] * a[13] + a[8] * a[1] * a[15] - a[8] * a[3] * a[13] - a[12] * a[1] * a[11] + a[12] * a[3] * a[9];
		inv[13] = a[0] * a[9] * a[14] - a[0] * a[10] * a[13] - a[8] * a[1] * a[14] + a[8] * a[2] * a[13] + a[12] * a[1] * a[10] - a[12] * a[2] * a[9];
		
		inv[2] = a[1] * a[6] * a[15] - a[1] * a[7] * a[14] - a[5] * a[2] * a[15] + a[5] * a[3] * a[14] + a[13] * a[2] * a[7] - a[13] * a[3] * a[6];
		inv[6] = -a[0] * a[6] * a[15] + a[0] * a[7] * a[14] + a[4] * a[2] * a[15] - a[4] * a[3] * a[14] - a[12] * a[2] * a[7] + a[12] * a[3] * a[6];
		inv[10] = a[0] * a[5] * a[15] - a[0] * a[7] * a[13] - a[4] * a[1] * a[15] + a[4] * a[3] * a[13] + a[12] * a[1] * a[7] - a[12] * a[3] * a[5];
		inv[14] = -a[0] * a[5] * a[14] + a[0] * a[6] * a[13] + a[4] * a[1] * a[14] - a[4] * a[2] * a[13] - a[12] * a[1] * a[6] + a[12] * a[2] * a[5];
		
		inv[3] = -a[1] * a[6] * a[11] + a[1] * a[7] * a[10] + a[5] * a[2] * a[11] - a[5] * a[3] * a[10] - a[9] * a[2] * a[7] + a[9] * a[3] * a[6];
		inv[7] = a[0] * a[6] * a[11] - a[0] * a[7] * a[10] - a[4] * a[2] * a[11] + a[4] * a[3] * a[10] + a[8] * a[2] * a[7] - a[8] * a[3] * a[6];
		inv[11] = -a[0] * a[5] * a[11] + a[0] * a[7] * a[9] + a[4] * a[1] * a[11] - a[4] * a[3] * a[9] - a[8] * a[1] * a[7] + a[8] * a[3] * a[5];
		inv[15] = a[0] * a[5] * a[10] - a[0] * a[6] * a[9] - a[4] * a[1] * a[10] + a[4] * a[2] * a[9] + a[8] * a[1] * a[6] - a[8] * a[2] * a[5];
		
		float det = a[0] * inv[0] + a[1] * inv[4] + a[2] * inv[8] + a[3] * inv[12];
		if (det == 0) {
			exit(0);
		}
		det = 1.0 / det;
		for (int i = 0; i < 16; i++) {
			inv[i] = inv[i] * det;
		}
		return inv;
	}
};

/* Quaternion Class */