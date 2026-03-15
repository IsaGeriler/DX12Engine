#pragma once

#include "../Core/MyMath.h"

// Collision Structures
class AABB {
public:
	Vec3 max;
	Vec3 min;

	AABB() { reset(); }
	
	void reset() {
		max = Vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		min = Vec3(FLT_MAX, FLT_MAX, FLT_MAX);
	}

	void extend(const Vec3& p) {
		max = Max(max, p);
		min = Min(min, p);
	}

	// Slab test
	bool ray_aabb(const Ray& r, float& t) {
		Vec3 s = (min - r.o) * r.invDir;
		Vec3 l = (max - r.o) * r.invDir;
		Vec3 s1 = Min(s, l);
		Vec3 l1 = Max(s, l);
		float ts = fmax(s1.x, fmax(s1.y, s1.z));
		float tl = fmin(s1.x, fmin(s1.y, s1.z));
		t = fmin(ts, tl);
		return (ts < tl);
	}
};

class Ray {
public:
	Vec3 o;		  // Origin of the ray
	Vec3 dir;	  // Direction of the ray
	Vec3 invDir;  // Inverse direction

	Ray() {}
	Ray(const Vec3& _o, const Vec3& _dir) { initialize(_o, _dir); }

	void initialize(const Vec3& _o, const Vec3& _dir) {
		o = _o;
		dir = _dir;
		invDir = Vec3(1.f, 1.f, 1.f) / dir;
	}

	// Ray = o + (t * d)
	Vec3 at(const float t) {
		return (o + (dir * t));
	}
};

class Sphere {
public:
	Vec3 centre;
	float radius;
};

// Collision Detections
bool aabb_aabb(const AABB& bbox1, const AABB& bbox2) {
	// Calculate Penetration Depths for all axes
	float pd_x = fmin(bbox1.max.x, bbox2.max.x) - fmax(bbox1.min.x, bbox2.min.x);
	float pd_y = fmin(bbox1.max.y, bbox2.max.y) - fmax(bbox1.min.y, bbox2.min.y);
	float pd_z = fmin(bbox1.max.z, bbox2.max.z) - fmax(bbox1.min.z, bbox2.min.z);

	return ((pd_x > 0) && (pd_y > 0) && (pd_z > 0));
}