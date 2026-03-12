#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>
#include <numbers>

#define USE_MATH_DEFINES
#define SQ(x) (x * x)

#undef min
#undef max

/* Templated Linear Interpolation */
template<typename Type>
static Type lerp(const Type a, const Type b, float t) {
	return (a * (1.f - t)) + (b * t);
}

/* Clamp */
template<typename Type>
static Type clamp(const Type val, const Type min, const Type max) {
	return std::max(std::min(val, max), min);
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

	void divideW() { float invW = 1.f / w; x *= invW; y *= invW; z *= invW; }

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
		memset(a, 0, 16 * sizeof(float));
		a[0] = 1.f; a[5] = 1.f; a[10] = 1.f; a[15] = 1.f;
 	}

	void zero() { memset(a, 0, 16 * sizeof(float)); }
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

	float& operator[](int index) {
		assert((index >= 0 && index <= 15) && "Invalid index passed!");
		return a[index];
	}

	Matrix transpose() {
		return Matrix(m[0][0], m[1][0], m[2][0], m[3][0],
					  m[0][1], m[1][1], m[2][1], m[3][1],
					  m[0][2], m[1][2], m[2][2], m[3][2],
					  m[0][3], m[1][3], m[2][3], m[3][3]
		);
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

	static Matrix perspective(float fov_theta, float aspect, float zNear, float zFar) {
		float fov = 1.f / tanf(fov_theta * 0.5f * std::numbers::pi_v<float> / 180.0f);
		float zNorm = 1.f / (zFar - zNear);

		Matrix pers;
		pers.zero();

		pers[0] = fov / aspect;
		pers[5] = fov;
		pers[10] = zFar * zNorm;
		pers[11] = -(zFar * zNear) * zNorm;
		pers[14] = 1.f;

		return pers;
	}

	static Matrix lookAt(const Vec3& from, const Vec3& to, const Vec3& up) {
		Matrix look;
		Vec3 dir = (to - from).normalize();
		Vec3 right = Cross(up, dir).normalize();
		Vec3 up1 = Cross(dir, right);

		look.m[0][0] = right.x;
		look.m[0][1] = right.y;
		look.m[0][2] = right.z;
		look.m[0][3] = -Dot(from, right);

		look.m[1][0] = up1.x;
		look.m[1][1] = up1.y;
		look.m[1][2] = up1.z;
		look.m[1][3] = -Dot(from, up1);

		look.m[2][0] = dir.x;
		look.m[2][1] = dir.y;
		look.m[2][2] = dir.z;
		look.m[2][3] = -Dot(from, dir);

		look.m[3][3] = 1;
		return look;
	}

	Vec4 mul(const Vec4& v) {
		return Vec4(v.x * a[0] + v.y * a[1] + v.z * a[2] + v.w + a[3],
					v.x * a[4] + v.y * a[5] + v.z * a[6] + v.w + a[7],
					v.x * a[8] + v.y * a[9] + v.z * a[10] + v.w + a[11],
					v.x * a[12] + v.y * a[13] + v.z * a[14] + v.w + a[15]
		);
	}

	Vec3 mulPoint(const Vec3& v) {
		return Vec3((v.x * a[0] + v.y * a[1] + v.z * a[2]) + a[3],
					(v.x * a[4] + v.y * a[5] + v.z * a[6]) + a[7],
					(v.x * a[8] + v.y * a[9] + v.z * a[10]) + a[11]
		);
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

	Matrix invert() const {
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
			// exit(0);
			inv.identity();
			return inv;
		}
		det = 1.f / det;
		for (int i = 0; i < 16; i++) {
			inv[i] = inv[i] * det;
		}
		return inv;
	}
};

/* Quaternion Class */
class Quaternion {
public:
	union {
		struct { float a, b, c, d; };
		float q[4];
	};

	Quaternion(float _a = 0.f, float _b = 0.f, float _c = 0.f, float _d = 0.f) : a(_a), b(_b), c(_c), d(_d) {}

	void conjugate() { a = -a; b = -b; c = -c; }
	float magnitude() const { return sqrtf(SQ(a) + SQ(b) + SQ(c) + SQ(d)); }

	void normalize() {
		float invMag = 1.f / sqrtf(SQ(a) + SQ(b) + SQ(c) + SQ(d));
		a *= invMag; b *= invMag; c *= invMag; d *= invMag;
	}

	void inverse() {
		conjugate();
		normalize();
	}

	Quaternion operator*(Quaternion q2) {
		return Quaternion((d * q2.a + a * q2.d + b * q2.c - c * q2.b),  // ai comp.
						  (d * q2.b - a * q2.c + b * q2.d + c * q2.a),  // bj comp.
						  (d * q2.b - a * q2.c + b * q2.d + c * q2.a),  // ck comp.
						  (d * q2.d - a * q2.a - b * q2.b - c * q2.c)   // d comp.
		);
	}

	Quaternion operator-() { return Quaternion(-a, -b, -c, -d); }

	Matrix toMatrix() const {
		float aa = a * a, ab = a * b, ac = a * c;
		float bb = b * b, bc = b * c, cc = c * c;
		float da = d * a, db = d * b, dc = d * c;

		Matrix m;
		m[0] = 1 - 2 * (bb + cc); m[1] = 2 * (ab - dc); m[2] = 2 * (ac + db); m[3] = 0;
		m[4] = 2 * (ab + dc); m[5] = 1 - 2 * (aa + cc); m[6] = 2 * (bc - da); m[7] = 0;
		m[8] = 2 * (ac - db); m[9] = 2 * (bc + da); m[10] = 1 - 2 * (aa + bb); m[11] = 0;
		m[12] = m[13] = m[14] = 0; m[15] = 1;
		return m;
	}

	void rotatePoint(Vec3& p, float theta, Vec3& v) {
		// q = c + vxsi + vysj + vzsk --> c = cos(theta/2), s = sin(theta/2)
		// pq = 0 + pxi + pyj + pzk
		// pq' = q pq q'

		Quaternion q, pq, qinv;
		q.a = v.x * sinf(0.5f * theta);
		q.b = v.y * sinf(0.5f * theta);
		q.c = v.z * sinf(0.5f * theta);
		q.d = cosf(0.5f * theta);

		pq.a = p.x; pq.b = p.y; pq.c = p.z; pq.d = 0.f;

		qinv = q;
		qinv.inverse();

		q = q * pq;
		q = q * qinv;

		a = q.a; b = q.b; c = q.c; d = q.d;
	}

	static Quaternion slerp(Quaternion& q1, Quaternion& q2, float t) {
		// q1.q2 = d1.d2 + a1.a2 + b1.b2 + c1.c2
		// theta = acos(q1.q2)
		// qInterpolated = sin(theta*(1-t))/sin(theta)q1 + sin(theta*t)/sin(theta)q2

		Quaternion interpolated;
		float dot = q1.d * q2.d + q1.a * q2.a + q1.b * q2.b + q1.c * q2.c;
		
		Quaternion q11 = dot < 0 ? -q1 : q1;
		dot = dot > 0 ? dot : -dot;

		float theta = acosf(clamp(dot, -1.f, 1.f));
		if (theta == 0.f) return q1;

		float a = sinf(theta * (1.f - t));
		float b = sinf(theta * t);
		float d = sinf(theta);

		float coeff1 = a / d;
		float coeff2 = b / d;

		interpolated.a = coeff1 * q11.a + coeff2 * q2.a;
		interpolated.b = coeff1 * q11.b + coeff2 * q2.b;
		interpolated.c = coeff1 * q11.c + coeff2 * q2.c;
		interpolated.d = coeff1 * q11.d + coeff2 * q2.d;
		
		interpolated.normalize();
		return interpolated;
	}
};

class ShadingFrame {
	// Given a unit vector n, find axis not colinear to n:
	// u' { (1 0 0)T if n != (1 0 0)T
	//		(0 1 0)T otherwise
	// 
	// u = (n x u') / |(n x u')|
	// v = n x u
public:
	Vec3 u;  // tangent vector (normalized)
	Vec3 v;  // binormal vector
	Vec3 w;  // normal vector (normalized)

	void fromVector(const Vec3& n) {
		// Gram-Schmidt Orthonormalization
		w = n.normalize();

		if (fabsf(w.x) > fabsf(w.y)) {
			float l = 1.f / sqrtf(w.x * w.x + w.z * w.z);
			u = Vec3(w.z * l, 0.f, -w.x * l);
		}
		else {
			float l = 1.f / sqrtf(w.y * w.y + w.z * w.z);
			u = Vec3(0.f, w.z * l, -w.y * l);
		}
		v = Cross(u, w);
	}

	void fromTangentVector(const Vec3& n, const Vec3& t) {
		w = n.normalize();
		u = t.normalize();
		v = Cross(u, w);
	}

	Vec3 toLocal(const Vec3& vec) const { return Vec3(Dot(u, vec), Dot(v, vec), Dot(w, vec)); }
	Vec3 toWorld(const Vec3& vec) const { return ((u * vec.x) + (v * vec.y) + (w * vec.z)); }
};

/* Colour Class */
class Colour {
public:
	union {
		struct { float r, g, b, a; };
		float c[4];
	};

	Colour(float _r = 0.f, float _g = 0.f, float _b = 0.f, float _a = 1.f) : r(_r), g(_g), b(_b), a(_a) {}
	Colour(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a = 255) : r(_r / 255.f), g(_g / 255.f), b(_b / 255.f), a(_a / 255.f) {}

	// Operator Overloading
	Colour operator+(const Colour& colour) const { return Colour(r + colour.r, g + colour.g, b + colour.b, a + colour.a); }
	Colour operator-(const Colour& colour) const { return Colour(r - colour.r, g - colour.g, b - colour.b, a - colour.a); }
	Colour operator*(const Colour& colour) const { return Colour(r * colour.r, g * colour.g, b * colour.b, a * colour.a); }
	Colour operator/(const Colour& colour) const { return Colour(r / colour.r, g / colour.g, b / colour.b, a / colour.a); }

	Colour operator*(const float scalar) const { return Colour(r * scalar, g * scalar, b * scalar, a * scalar); }
	Colour operator/(const float scalar) const { return Colour(r / scalar, g / scalar, b / scalar, a / scalar); }
};