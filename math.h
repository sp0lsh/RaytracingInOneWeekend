#pragma once

#include "vec3.h"

#define MAXFLOAT 100000.0f
#define M_PI 3.1515926f

class material;

class ray
{
public:
	ray() {}
	ray(const vec3& a, const vec3& b) { A = a; B = b; }
	vec3 origin() const { return A; }
	vec3 dir() const { return B; }
	vec3 pointAtT(float t) const { return A + t * B; }

	vec3 A;
	vec3 B;
};

class camera {
public:
	// vfov: top-bottom, degrees
	camera(const vec3& lookFrom, const vec3& lookAt, const vec3& up, float vfov, float aspect, float aperture, float focusDisk) {

		lensRadius = aperture / 2.0f;

		float theta = vfov * M_PI / 180.0f;
		float halfHeight = tan(theta / 2.0f);
		float halfWidth = aspect * halfHeight;
		
		origin = lookFrom;
		w = normalize(lookFrom - lookAt);
		u = normalize(cross(up, w));
		v = cross(w, u);
		
		lowerLeftCorner = origin - halfWidth * focusDisk * u - halfHeight * focusDisk * v - focusDisk * w;
		horizontal = 2.0f * halfWidth * focusDisk * u;
		vertical = 2.0f * halfHeight * focusDisk * v;
	}

	ray getRay(float s, float t) {
		vec3 rd = lensRadius * randomInUnitDisk();
		vec3  offset = u * rd.x() + v * rd.y();
		return ray(origin + offset, lowerLeftCorner + s * horizontal + t * vertical - origin - offset);
	}

	vec3 origin;
	vec3 lowerLeftCorner;
	vec3 horizontal;
	vec3 vertical;
	vec3 u, v, w;
	float lensRadius;
};

class material;

struct hitRecord {
	float t;
	vec3 point;
	vec3 normal;
	material *mat;
};

class hitable {
public:
	virtual bool hit(const ray &r, float tMin, float tMax, hitRecord &hit) const = 0;
};


class sphere : public hitable {
public:
	sphere() {}
	sphere(vec3 cen, float r, material* m) : center(cen), radius(r), mat(m) {};
	virtual bool hit(const ray& r, float tMin, float tMax, hitRecord &hit) const;

	vec3  center;
	float radius;
	material* mat;
};

bool sphere::hit(const ray & r, float tMin, float tMax, hitRecord & hit) const
{
	vec3 oc = r.origin() - center;
	float a = dot(r.dir(), r.dir());
	float b = dot(oc, r.dir());
	float c = dot(oc, oc) - radius * radius;
	float delta = b * b - a * c;
	if (delta > 0.0f) {
		float temp = (-b - sqrt(b*b - a * c)) / a;
		if (temp < tMax && temp > tMin) {
			hit.t = temp;
			hit.point = r.pointAtT(hit.t);
			hit.normal = (hit.point - center) / radius;
			hit.mat = mat;
			return true;
		}
		temp = (-b + sqrt(b*b - a * c)) / a;
		if (temp < tMax && temp > tMin) {
			hit.t = temp;
			hit.point = r.pointAtT(hit.t);
			hit.normal = (hit.point - center) / radius;
			hit.mat = mat;
			return true;
		}
	}
	return false;
}

class hitableList : public hitable {
public:
	hitableList() {}
	hitableList(hitable **l, int n) { list = l; listSize = n; }
	virtual bool hit(const ray & r, float tMin, float tMax, hitRecord & hit) const override;

	hitable** list;
	int listSize;
};

bool hitableList::hit(const ray & r, float tMin, float tMax, hitRecord & hit) const
{
	hitRecord tempHit;
	bool isHit = false;
	double clossestHitable = tMax;
	for (int i = 0; i < listSize; i++) {
		if (list[i]->hit(r, tMin, clossestHitable, tempHit)) {
			isHit = true;
			clossestHitable = tempHit.t;
			hit = tempHit;
		}
	}
	return isHit;
}