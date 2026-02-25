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

/* Quaternion Class */